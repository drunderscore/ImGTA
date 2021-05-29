/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "mod.h"
#include "types.h"

class HandleHelperMod : public Mod
{
public:
    HandleHelperMod() : Mod( "Handle Helper", true )
    {
        m_iWindowFlags = ImGuiWindowFlags_MenuBar;
        m_position.x = 0.0f;
        m_position.y = 0.0f;
        m_position.z = 0.0f;
    }

    bool Draw() override;
    void Think() override;
    void Load() override;
    void Unload() override;

private:
    void ShowMenuBar();
    void UpdateHandleData( bool once = false );

    int m_iType = 0;
    int m_iHealth = 0;
    int m_iMaxHealth = 0;
    Hash m_modelHash = 0;
    Vector3 m_position;
    bool m_bIsMissionEntity = false;


    // ImGui inputs / internals
    bool m_bWantsUpdate = false;
    int m_iHandleInput = 0;
    bool m_bConstantUpdate = true;
    char m_szAnimDictInput[256] = "";
    char m_szAnimNameInput[256] = "";
    unsigned int m_iAnimFlags = 0;
    bool m_bAnimLoop = false;
};