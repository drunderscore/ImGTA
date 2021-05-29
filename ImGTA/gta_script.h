/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "types.h"

class GTAScript
{
public:
    GTAScript( const char *scriptName, bool block = true );
    ~GTAScript();

    void Unload();
    const char *GetScriptName();
    bool IsValid();
    void WaitForLoad();

private:
    bool m_bIsValid = false;
    bool m_bMarked = false;
    char m_szScriptName[128] = "";
};