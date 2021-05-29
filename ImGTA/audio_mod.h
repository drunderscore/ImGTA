/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "mod.h"
#include "types.h"

class AudioMod : public Mod
{
public:
    AudioMod() : Mod( "Audio", true )
    {
        m_iWindowFlags = ImGuiWindowFlags_MenuBar;
    }

    bool Draw() override;
    void Think() override;
    void Load() override;
    void Unload() override;

private:
    void DrawMenuBar();

    bool m_bIsRingtonePlaying = false;
    bool m_bIsCallOngoing = false;

    bool m_bIsScriptedConversationLoaded = false;
    bool m_bIsScriptedConversationOngoing = false;
    bool m_bIsInCurrentConversation = false;
    int m_iCurrentScriptedConversationLine = 0;

    bool m_bIsStreamPlaying = false;
    int m_iStreamPlayTime = 0;

    bool m_bIsScriptedMusicPlaying = false;

    bool m_bIsAmbientSpeechPlaying = false;
    bool m_bIsScriptedSpeechPlaying = false;
    bool m_bIsAnySpeechPlaying = false;

    // ImGui inputs
    bool m_bStopConversationParam = false;
    bool m_bPauseConversationParam = false;
    char m_szAudioName[256] = "";
};