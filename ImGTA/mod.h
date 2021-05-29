/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include <string>
#include "imgui.h"

class Mod
{
public:
    Mod( std::string name, bool hasWindow );
    bool HasWindow();
    const std::string GetName();

    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual void Think() = 0;
    // Draws in an ImGui window, already created. Return false to NOT automatically end the window.
    virtual bool Draw() = 0;

    ImGuiWindowFlags m_iWindowFlags;
private:
    std::string m_szName;
    bool m_bHasWindow;
};