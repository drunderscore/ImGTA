/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "mod.h"
#include "lua_engine.h"
#include "types.h"

class TestMod : public Mod
{
public:
    TestMod() : Mod( "Test", true )
    {
        m_iWindowFlags = ImGuiWindowFlags_None;
    }

    bool Draw() override;
    void Think() override;
    void Load() override;
    void Unload() override;

private:

};