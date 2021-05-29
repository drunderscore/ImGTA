/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "mod.h"
#include <sstream>
#include <vector>
#include <string>

class LuaConsoleMod : public Mod
{
public:
    LuaConsoleMod() : Mod( "Lua Console", true )
    {
        m_iWindowFlags = ImGuiWindowFlags_MenuBar;
    }

    bool Draw() override;
    void Think() override;
    void Load() override;
    void Unload() override;

private:
    void ShowMenuBar();
    char m_szInput[1024] = "";
    char m_szLuaInput[1024] = "";
    int m_iCurrentHistory = 0;
    std::vector<std::string> m_commandHistory;
};