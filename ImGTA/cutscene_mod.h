/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "mod.h"
#include "types.h"

class CutsceneMod : public Mod
{
public:
    CutsceneMod() : Mod( "Cutscene", true )
    {
        m_iWindowFlags = ImGuiWindowFlags_MenuBar;
        m_startPos.x = 0;
        m_startPos.y = 0;
        m_startPos.z = 0;
    }

    bool Draw() override;
    void Think() override;
    void Load() override;
    void Unload() override;

private:
    char m_szRequestedCutscene[256] = "";
    void ShowMenuBar();

    bool m_bHasCutsceneLoaded = false;
    bool m_bHasCutsceneFinished = false;
    bool m_bIsCutscenePlaying = false;
    bool m_bIsCutsceneActive = false;

    int m_iCutsceneTime = 0;
    int m_iCutsceneTotalDuration = 0;
    int m_iCutsceneSectionPlaying = 0;

    // ImGui inputs
    unsigned int m_iCutsceneRequestFlags = 8;
    unsigned int m_iCutsceneStartFlags = 0;
    char m_szCutsceneInput[256] = "";

    Vector3 m_startPos;
};