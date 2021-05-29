/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "mod.h"
#include "mission_helper.h"
#include "types.h"

class CheatsMod : public Mod
{
public:
    CheatsMod() : Mod( "Cheats", false )
    {
        m_teleportPos.x = 0;
        m_teleportPos.y = 0;
        m_teleportPos.z = 0;
    }

    bool Draw() override;
    void Think() override;
    void Load() override;
    void Unload() override;

private:
    void DrawPlayerMenu();
    void DrawVehicleMenu();
    void DrawWorldMenu();
    void DrawMissionMenu();
    void DrawHUDMenu();

    bool m_bExplosiveBullets = false;
    bool m_bShowDebug = false;

    int m_iLargestStreaming = 0;
    int m_iLargestStreamingTime = 0;

    Vector3 m_teleportPos;

    char m_szVehicleHashInput[256] = "";

    // TODO: create spawn history, allow more stuff.
    Entity m_lastSpawned = 0;

    MissionHelper *m_helper = nullptr;
};