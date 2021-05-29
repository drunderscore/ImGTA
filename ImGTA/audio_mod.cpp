/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "audio_mod.h"
#include "natives.h"
#include "script.h"

void AudioMod::Load()
{

}

void AudioMod::Unload()
{

}

void AudioMod::Think()
{
    Ped p = PLAYER::PLAYER_PED_ID();

    m_bIsRingtonePlaying = AUDIO::IS_PED_RINGTONE_PLAYING( p );
    m_bIsCallOngoing = AUDIO::IS_MOBILE_PHONE_CALL_ONGOING();

    m_bIsScriptedConversationLoaded = AUDIO::IS_SCRIPTED_CONVERSATION_LOADED();
    m_bIsScriptedConversationOngoing = AUDIO::IS_SCRIPTED_CONVERSATION_ONGOING();
    m_bIsInCurrentConversation = AUDIO::IS_PED_IN_CURRENT_CONVERSATION( p );
    m_iCurrentScriptedConversationLine = AUDIO::GET_CURRENT_SCRIPTED_CONVERSATION_LINE();

    m_bIsStreamPlaying = AUDIO::IS_STREAM_PLAYING();
    m_iStreamPlayTime = AUDIO::GET_STREAM_PLAY_TIME();

    m_bIsScriptedMusicPlaying = AUDIO::AUDIO_IS_SCRIPTED_MUSIC_PLAYING();

    m_bIsAmbientSpeechPlaying = AUDIO::IS_AMBIENT_SPEECH_PLAYING( p );
    m_bIsScriptedSpeechPlaying = AUDIO::IS_SCRIPTED_SPEECH_PLAYING( p );
    m_bIsAnySpeechPlaying = AUDIO::IS_ANY_SPEECH_PLAYING( p );
}

void AudioMod::DrawMenuBar()
{
    if ( ImGui::BeginMenuBar() )
    {
        if ( ImGui::BeginMenu( "Conversation" ) )
        {
            if ( ImGui::MenuItem( "Restart" ) )
            {
                RunOnNativeThread( []
                {
                    AUDIO::RESTART_SCRIPTED_CONVERSATION();
                } );
            }

            if ( ImGui::BeginMenu( "Stop" ) )
            {
                ImGui::Checkbox( "Unknown param", &m_bStopConversationParam );
                if ( ImGui::Button( "Stop" ) )
                {
                    RunOnNativeThread( [ = ]
                    {
                        AUDIO::STOP_SCRIPTED_CONVERSATION( m_bStopConversationParam );
                    } );
                }

                ImGui::EndMenu();
            }

            if ( ImGui::BeginMenu( "Pause" ) )
            {
                ImGui::Checkbox( "Finish Current Line", &m_bPauseConversationParam );

                if ( ImGui::Button( "Pause" ) )
                {
                    RunOnNativeThread( [ = ]
                    {
                        AUDIO::PAUSE_SCRIPTED_CONVERSATION( m_bPauseConversationParam );
                    } );
                }

                ImGui::EndMenu();
            }

            if ( ImGui::MenuItem( "Skip to Next Line" ) )
            {
                RunOnNativeThread( [ = ]
                {
                    AUDIO::SKIP_TO_NEXT_SCRIPTED_CONVERSATION_LINE();
                } );
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

bool AudioMod::Draw()
{
    DrawMenuBar();

    ImGui::Text( "Is Phone Ringing: %d", m_bIsRingtonePlaying );
    ImGui::Text( "Is Call Ongoing: %d", m_bIsCallOngoing );
    ImGui::Separator();
    ImGui::Text( "Is Scripted Conversation Loaded: %d", m_bIsScriptedConversationLoaded );
    ImGui::Text( "Is Scripted Conversation Ongoing: %d", m_bIsScriptedConversationOngoing );
    ImGui::Text( "Is In Current Conversation: %d", m_bIsInCurrentConversation );
    ImGui::Text( "Current Conversation Line: %d", m_iCurrentScriptedConversationLine );
    ImGui::Separator();
    ImGui::Text( "Is Stream Playing: %d", m_bIsStreamPlaying );
    ImGui::Text( "Stream Play Time: %d", m_iStreamPlayTime );
    ImGui::Separator();
    ImGui::Text( "Is Scripted Music Playing: %d", m_bIsScriptedMusicPlaying );
    ImGui::Separator();
    ImGui::Text( "Is Ambient Speech Playing: %d", m_bIsAmbientSpeechPlaying );
    ImGui::Text( "Is Scripted Speech Playing: %d", m_bIsScriptedSpeechPlaying );
    ImGui::Text( "Is Any Speech Playing: %d", m_bIsAnySpeechPlaying );
    return true;
}