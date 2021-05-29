/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "handle_helper_mod.h"
#include "natives.h"
#include "script.h"
#include "anim_dict.h"
#include "imgui_extras.h"

const char *entityTypes[] = { "Invalid", "Ped", "Vehicle", "Object" };

void HandleHelperMod::Load()
{

}

void HandleHelperMod::Unload()
{

}

void HandleHelperMod::Think()
{
    if ( ( m_bConstantUpdate || m_bWantsUpdate ) && ENTITY::DOES_ENTITY_EXIST( m_iHandleInput ) )
    {
        UpdateHandleData( m_bWantsUpdate );
        m_bWantsUpdate = false;
    }
}

void HandleHelperMod::UpdateHandleData( bool once )
{
    if ( once )
        m_modelHash = ENTITY::GET_ENTITY_MODEL( m_iHandleInput );

    m_iType = ENTITY::GET_ENTITY_TYPE( m_iHandleInput );
    m_iHealth = ENTITY::GET_ENTITY_HEALTH( m_iHandleInput );
    m_iMaxHealth = ENTITY::GET_ENTITY_MAX_HEALTH( m_iHandleInput );
    m_position = ENTITY::GET_ENTITY_COORDS( m_iHandleInput, TRUE );
    m_bIsMissionEntity = ENTITY::IS_ENTITY_A_MISSION_ENTITY( m_iHandleInput );
}

void HandleHelperMod::ShowMenuBar()
{
    if ( ImGui::BeginMenuBar() )
    {
        if ( ImGui::BeginMenu( "Entity" ) )
        {
            if ( ImGui::BeginMenu( "Persistence" ) )
            {
                if ( ImGui::MenuItem( "Set Mission Entity" ) )
                {
                    RunOnNativeThread( [ = ]
                    {
                        if ( ENTITY::DOES_ENTITY_EXIST( m_iHandleInput ) )
                            ENTITY::SET_ENTITY_AS_MISSION_ENTITY( m_iHandleInput, true, false );
                    } );
                }

                if ( ImGui::MenuItem( "Mark as No Longer Needed" ) )
                {
                    RunOnNativeThread( [ = ]
                    {
                        if ( ENTITY::DOES_ENTITY_EXIST( m_iHandleInput ) )
                        {
                            int tempHandle = m_iHandleInput;
                            ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED( &tempHandle );
                        }
                    } );
                }

                ImGui::EndMenu();
            }

            if ( ImGui::MenuItem( "Kill" ) )
            {
                RunOnNativeThread( [ = ]
                {
                    if ( ENTITY::DOES_ENTITY_EXIST( m_iHandleInput ) )
                        ENTITY::SET_ENTITY_HEALTH( m_iHandleInput, 0 );
                } );
            }

            if ( ImGui::MenuItem( "Delete" ) )
            {
                RunOnNativeThread( [ = ]
                {
                    if ( ENTITY::DOES_ENTITY_EXIST( m_iHandleInput ) )
                        ENTITY::DELETE_ENTITY( &m_iHandleInput );
                } );
            }

            if ( ImGui::BeginMenu( "Animation" ) )
            {
                if ( ImGui::BeginMenu( "Play" ) )
                {
                    ImGui::InputText( "Anim Dict", m_szAnimDictInput, sizeof( m_szAnimDictInput ) );
                    ImGui::InputText( "Anim Name", m_szAnimNameInput, sizeof( m_szAnimNameInput ) );
                    ImGuiExtras::BitField( "Flags", &m_iAnimFlags, nullptr );
                    if ( ImGui::Button( "Play" ) )
                    {
                        RunOnNativeThread( [ = ]
                        {
                            if ( ENTITY::DOES_ENTITY_EXIST( m_iHandleInput ) )
                            {
                                AnimDict dict( m_szAnimDictInput );
                                if ( dict.IsValid() )
                                    AI::TASK_PLAY_ANIM( m_iHandleInput, m_szAnimDictInput, m_szAnimNameInput, 8.0f, 1.0f, -1, m_iAnimFlags, 1.0f, FALSE, FALSE, FALSE );
                            }
                        } );
                    }
                    ImGui::EndMenu();
                }

                if ( ImGui::MenuItem( "Stop" ) )
                {
                    RunOnNativeThread( [ = ]
                    {
                        if ( ENTITY::DOES_ENTITY_EXIST( m_iHandleInput ) )
                            AI::STOP_ANIM_PLAYBACK( m_iHandleInput, 0, 0 );
                    } );
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

bool HandleHelperMod::Draw()
{
    ShowMenuBar();

    if ( ImGui::InputInt( "Handle", &m_iHandleInput ) )
        m_bWantsUpdate = true;

    ImGui::Checkbox( "Constant Updates?", &m_bConstantUpdate );
    if ( !m_bConstantUpdate )
        if ( ImGui::Button( "Update" ) )
            m_bWantsUpdate = true;

    ImGui::Separator();

    ImGui::Text( "Entity Type: %s (%d)", entityTypes[m_iType], m_iType );
    ImGui::Text( "Health: %d / %d", m_iHealth, m_iMaxHealth );
    ImGui::Text( "Model: %d (0x%x)", m_modelHash, m_modelHash );
    ImGui::Text( "Position: %.2f, %.2f, %.2f", m_position.x, m_position.y, m_position.z );
    ImGui::Text( "Is Mission Entity: %d", m_bIsMissionEntity );
    return true;
}