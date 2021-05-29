/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "cutscene_mod.h"
#include "natives.h"
#include "script.h"
#include "imgui_extras.h"

void CutsceneMod::Load()
{

}

void CutsceneMod::Unload()
{

}

void CutsceneMod::Think()
{
    m_bHasCutsceneLoaded = CUTSCENE::HAS_CUTSCENE_LOADED();
    m_bHasCutsceneFinished = CUTSCENE::HAS_CUTSCENE_FINISHED();
    m_bIsCutscenePlaying = CUTSCENE::IS_CUTSCENE_PLAYING();
    m_bIsCutsceneActive = CUTSCENE::IS_CUTSCENE_ACTIVE();

    m_iCutsceneTime = CUTSCENE::GET_CUTSCENE_TIME();
    m_iCutsceneTotalDuration = CUTSCENE::GET_CUTSCENE_TOTAL_DURATION();
    m_iCutsceneSectionPlaying = CUTSCENE::GET_CUTSCENE_SECTION_PLAYING();
}

void CutsceneMod::ShowMenuBar()
{
    if ( ImGui::BeginMenuBar() )
    {
        if ( ImGui::BeginMenu( "Stream" ) )
        {
            if ( ImGui::BeginMenu( "Load" ) )
            {
                if ( ImGui::InputText( "Name##LoadCutscene", m_szCutsceneInput, sizeof( m_szCutsceneInput ), ImGuiInputTextFlags_EnterReturnsTrue ) )
                {
                    strcpy_s( m_szRequestedCutscene, m_szCutsceneInput );
                    RunOnNativeThread( [ = ]
                    {
                        CUTSCENE::REQUEST_CUTSCENE( m_szRequestedCutscene, m_iCutsceneRequestFlags );
                    } );
                }
                ImGuiExtras::BitField( "Flags##LoadCutscene", &m_iCutsceneRequestFlags, nullptr );

                ImGui::EndMenu();
            }

            if ( ImGui::MenuItem( "Unload" ) )
            {
                RunOnNativeThread( []
                {
                    CUTSCENE::REMOVE_CUTSCENE();
                } );
            }

            ImGui::EndMenu();
        }

        if ( ImGui::BeginMenu( "Cutscene" ) )
        {
            if ( ImGui::BeginMenu( "Start" ) )
            {
                ImGuiExtras::BitField( "Flags##StartCutscene", &m_iCutsceneStartFlags, nullptr );

                if ( ImGui::MenuItem( "Start##CutsceneStart" ) )
                {
                    RunOnNativeThread( [ = ]
                    {
                        CUTSCENE::START_CUTSCENE( m_iCutsceneStartFlags );
                    } );
                }

                if ( ImGui::BeginMenu( "At Position" ) )
                {
                    ImGuiExtras::InputVector3( "CutsceneStartPosition", &m_startPos );
                    if ( ImGui::Button( "Set to current position" ) )
                    {
                        RunOnNativeThread( [ = ]
                        {
                            m_startPos = ENTITY::GET_ENTITY_COORDS( PLAYER::PLAYER_PED_ID(), TRUE );
                        } );
                    }

                    if ( ImGui::Button( "Start##CutsceneStartAtPosition" ) )
                    {
                        RunOnNativeThread( [ = ]
                        {
                            CUTSCENE::START_CUTSCENE_AT_COORDS( m_startPos.x, m_startPos.y, m_startPos.z, m_iCutsceneStartFlags );
                        } );
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            if ( ImGui::BeginMenu( "Stop" ) )
            {
                if ( ImGui::MenuItem( "Stop##StopCutsceneDefault" ) )
                {
                    RunOnNativeThread( []
                    {
                        CUTSCENE::STOP_CUTSCENE( TRUE );
                    } );
                }

                if ( ImGui::MenuItem( "Stop Immediately" ) )
                {
                    RunOnNativeThread( []
                    {
                        CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
                    } );
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

bool CutsceneMod::Draw()
{
    ShowMenuBar();

    ImGui::Text( "Has Loaded: %d", m_bHasCutsceneLoaded );
    ImGui::Text( "Has Finished: %d", m_bHasCutsceneFinished );
    ImGui::Text( "Is Playing: %d", m_bIsCutscenePlaying );
    ImGui::Text( "Is Active: %d", m_bIsCutsceneActive );
    ImGui::Text( "TIme: %d", m_iCutsceneTime );
    ImGui::Text( "Duration: %d", m_iCutsceneTotalDuration );
    ImGui::Text( "Section: %d", m_iCutsceneSectionPlaying );

    return true;
}