/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "mod.h"

Mod::Mod( std::string name, bool hasWindow ) : m_szName( name ), m_bHasWindow( hasWindow )
{

}

bool Mod::HasWindow()
{
    return m_bHasWindow;
}

const std::string Mod::GetName()
{
    return m_szName;
}