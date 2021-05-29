/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "cheats_mod.h"
#include "natives.h"
#include "script.h"
#include "utils.h"
#include "imgui_extras.h"
#include "model.h"

#pragma region Weapon Array
const char *weapons[] = { "weapon_dagger", "weapon_bat", "weapon_bottle", "weapon_crowbar", "weapon_unarmed", "weapon_flashlight", "weapon_golfclub", "weapon_hammer", "weapon_hatchet", "weapon_knuckle", "weapon_knife", "weapon_machete", "weapon_switchblade", "weapon_nightstick", "weapon_wrench", "weapon_battleaxe", "weapon_poolcue", "weapon_stone_hatchet", "weapon_pistol", "weapon_pistol_mk2", "weapon_combatpistol", "weapon_appistol", "weapon_stungun", "weapon_pistol50", "weapon_snspistol", "weapon_snspistol_mk2", "weapon_heavypistol", "weapon_vintagepistol", "weapon_flaregun", "weapon_marksmanpistol", "weapon_revolver", "weapon_revolver_mk2", "weapon_doubleaction", "weapon_raypistol", "weapon_ceramicpistol", "weapon_navyrevolver", "weapon_microsmg", "weapon_smg", "weapon_smg_mk2", "weapon_assaultsmg", "weapon_combatpdw", "weapon_machinepistol", "weapon_minismg", "weapon_raycarbine", "weapon_pumpshotgun", "weapon_pumpshotgun_mk2", "weapon_sawnoffshotgun", "weapon_assaultshotgun", "weapon_bullpupshotgun", "weapon_musket", "weapon_heavyshotgun", "weapon_dbshotgun", "weapon_autoshotgun", "weapon_assaultrifle", "weapon_assaultrifle_mk2", "weapon_carbinerifle", "weapon_carbinerifle_mk2", "weapon_advancedrifle", "weapon_specialcarbine", "weapon_specialcarbine_mk2", "weapon_bullpuprifle", "weapon_bullpuprifle_mk2", "weapon_compactrifle", "weapon_mg", "weapon_combatmg", "weapon_combatmg_mk2", "weapon_gusenberg", "weapon_sniperrifle", "weapon_heavysniper", "weapon_heavysniper_mk2", "weapon_marksmanrifle", "weapon_marksmanrifle_mk2", "weapon_rpg", "weapon_grenadelauncher", "weapon_grenadelauncher_smoke", "weapon_minigun", "weapon_firework", "weapon_railgun", "weapon_hominglauncher", "weapon_compactlauncher", "weapon_rayminigun", "weapon_grenade", "weapon_bzgas", "weapon_molotov", "weapon_stickybomb", "weapon_proxmine", "weapon_snowball", "weapon_pipebomb", "weapon_ball", "weapon_smokegrenade", "weapon_flare", "weapon_petrolcan", "gadget_parachute", "weapon_fireextinguisher", "weapon_hazardcan" };
#pragma endregion

void CheatsMod::Load()
{
    //m_helper = new MissionHelper( getGameVersion() );
}

void CheatsMod::Unload()
{

}

void CheatsMod::Think()
{
    if ( m_bExplosiveBullets )
        GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME( PLAYER::PLAYER_ID() );

    char buf[256] = "";
    if ( m_bShowDebug )
    {
        Vector3 pos = ENTITY::GET_ENTITY_COORDS( PLAYER::PLAYER_PED_ID(), TRUE );
        sprintf_s( buf, "Position: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z );
        DrawTextToScreen( buf, 0.155f, 0.81f, 0.3f, eFont::FontChaletLondon );
        sprintf_s( buf, "Game Time: %d", GAMEPLAY::GET_GAME_TIMER() );
        DrawTextToScreen( buf, 0.155f, 0.83f, 0.3f, eFont::FontChaletLondon );

        int streamingCount = STREAMING::GET_NUMBER_OF_STREAMING_REQUESTS();

        if ( streamingCount >= m_iLargestStreaming )
        {
            m_iLargestStreaming = streamingCount;
            m_iLargestStreamingTime = GAMEPLAY::GET_GAME_TIMER();
        }

        sprintf_s( buf, "Streaming: %d (%d)", streamingCount, m_iLargestStreaming );
        DrawTextToScreen( buf, 0.155f, 0.85f, 0.3f, eFont::FontChaletLondon );

        if ( m_iLargestStreaming != 0 && GAMEPLAY::GET_GAME_TIMER() >= m_iLargestStreamingTime + 3000 )
            m_iLargestStreaming = 0;

        sprintf_s( buf, "Your Ped Handle: %d", PLAYER::PLAYER_PED_ID() );
        DrawTextToScreen( buf, 0.155f, 0.87f, 0.3f, eFont::FontChaletLondon );
    }
}

void CheatsMod::DrawPlayerMenu()
{
    if ( ImGui::BeginMenu( "Player" ) )
    {
        if ( ImGui::MenuItem( "Heal" ) )
        {
            RunOnNativeThread( []
            {
                auto p = PLAYER::PLAYER_PED_ID();
                ENTITY::SET_ENTITY_HEALTH( p, PED::GET_PED_MAX_HEALTH( p ) );
            } );
        }

        if ( ImGui::MenuItem( "Kill" ) )
        {
            RunOnNativeThread( []
            {
                ENTITY::SET_ENTITY_HEALTH( PLAYER::PLAYER_PED_ID(), 0 );
            } );
        }

        if ( ImGui::BeginMenu( "Teleport" ) )
        {
            ImGuiExtras::InputVector3( "TeleportPos", &m_teleportPos );
            if ( ImGui::Button( "Teleport" ) )
            {
                RunOnNativeThread( [ = ]
                {
                    ENTITY::SET_ENTITY_COORDS( PLAYER::PLAYER_PED_ID(), m_teleportPos.x, m_teleportPos.y, m_teleportPos.z, TRUE, TRUE, TRUE, FALSE );
                } );
            }
            ImGui::EndMenu();
        }

        if ( ImGui::BeginMenu( "Weapon" ) )
        {
            ImGui::MenuItem( "Explosive Bullets", NULL, &m_bExplosiveBullets );

            if ( ImGui::MenuItem( "Clear" ) )
            {
                RunOnNativeThread( []
                {
                    WEAPON::REMOVE_ALL_PED_WEAPONS( PLAYER::PLAYER_PED_ID(), FALSE );
                } );
            }

            if ( ImGui::MenuItem( "Give All" ) )
            {
                RunOnNativeThread( []
                {
                    for ( int i = 0; i < IM_ARRAYSIZE( weapons ); i++ )
                    {
                        WEAPON::GIVE_WEAPON_TO_PED( PLAYER::PLAYER_PED_ID(), GAMEPLAY::GET_HASH_KEY( ( char * )weapons[i] ), 99999, FALSE, TRUE );
                    }
                } );
            }

            if ( ImGui::BeginMenu( "Give..." ) )
            {
                for ( int i = 0; i < IM_ARRAYSIZE( weapons ); i++ )
                {
                    if ( ImGui::MenuItem( weapons[i] ) )
                    {
                        RunOnNativeThread( [i]
                        {
                            WEAPON::GIVE_WEAPON_TO_PED( PLAYER::PLAYER_PED_ID(), GAMEPLAY::GET_HASH_KEY( ( char * )weapons[i] ), 99999, FALSE, TRUE );
                        } );
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void CheatsMod::DrawVehicleMenu()
{
    if ( ImGui::BeginMenu( "Vehicle" ) )
    {
        if ( ImGui::MenuItem( "Repair" ) )
        {
            RunOnNativeThread( []
            {
                Vehicle v = PED::GET_VEHICLE_PED_IS_IN( PLAYER::PLAYER_PED_ID(), FALSE );
                if ( v != NULL )
                    VEHICLE::SET_VEHICLE_FIXED( v );
            } );
        }

        if ( ImGui::BeginMenu( "Destroy##Vehicle" ) )
        {
            if ( ImGui::MenuItem( "Kill" ) )
            {
                RunOnNativeThread( []
                {
                    Vehicle v = PED::GET_VEHICLE_PED_IS_IN( PLAYER::PLAYER_PED_ID(), FALSE );
                    if ( v != NULL )
                    {
                        VEHICLE::SET_VEHICLE_ENGINE_HEALTH( v, 0.0f );
                        VEHICLE::SET_VEHICLE_BODY_HEALTH( v, 0.0f );
                        ENTITY::SET_ENTITY_HEALTH( v, 0 );
                    }
                } );
            }

            if ( ImGui::MenuItem( "Explode" ) )
            {
                RunOnNativeThread( []
                {
                    Vehicle v = PED::GET_VEHICLE_PED_IS_IN( PLAYER::PLAYER_PED_ID(), FALSE );
                    if ( v != NULL )
                        VEHICLE::EXPLODE_VEHICLE( v, TRUE, FALSE );
                } );
            }

            ImGui::EndMenu();
        }

        if ( ImGui::MenuItem( "Customize" ) )
        {
            // TODO: show customization window
        }

        ImGui::EndMenu();
    }
}

void CheatsMod::DrawWorldMenu()
{
    if ( ImGui::BeginMenu( "World" ) )
    {
        if ( m_lastSpawned != 0 )
            ImGui::Text( "Last Spawned Handle: %d", m_lastSpawned );
        if ( ImGui::BeginMenu( "Spawn Vehicle" ) )
        {
            if ( ImGui::BeginMenu( "Hash##SpawnVehicleMenu" ) )
            {
                if ( ImGui::InputText( "Hash##SpawnVehicleInput", m_szVehicleHashInput, sizeof( m_szVehicleHashInput ), ImGuiInputTextFlags_EnterReturnsTrue ) )
                {
                    RunOnNativeThread( [ = ]
                    {
                        Model vehModel( m_szVehicleHashInput );
                        if ( vehModel.IsValid() && vehModel.IsVehicle() )
                        {
                            Vector3 pos = ENTITY::GET_ENTITY_COORDS( PLAYER::PLAYER_PED_ID(), TRUE );
                            m_lastSpawned = VEHICLE::CREATE_VEHICLE( vehModel.GetHash(), pos.x, pos.y, pos.z, 0.0f, TRUE, TRUE );
                        }
                    } );
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
}

void CheatsMod::DrawMissionMenu()
{
    if ( ImGui::BeginMenu( "Mission" ) )
    {
        if ( m_helper == nullptr )
            ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ), "Version unsupported." );
        else
        {
            if ( ImGui::MenuItem( "Skip" ) )
                m_helper->Skip();
        }

        ImGui::EndMenu();
    }
}

void CheatsMod::DrawHUDMenu()
{
    if ( ImGui::BeginMenu( "HUD" ) )
    {
        ImGui::MenuItem( "Show Debug", NULL, &m_bShowDebug );
        ImGui::EndMenu();
    }
}

bool CheatsMod::Draw()
{
    if ( ImGui::BeginMainMenuBar() )
    {
        DrawPlayerMenu();
        DrawVehicleMenu();
        DrawWorldMenu();
        DrawMissionMenu();
        DrawHUDMenu();
        ImGui::EndMainMenuBar();
    }

    return false;
}