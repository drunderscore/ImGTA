/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "gta_script.h"
#include "natives.h"

GTAScript::GTAScript( const char *scriptName, bool block )
{
    strcpy_s( m_szScriptName, scriptName );
    m_bIsValid = SCRIPT::DOES_SCRIPT_EXIST( m_szScriptName );

    if ( IsValid() )
    {
        SCRIPT::REQUEST_SCRIPT( m_szScriptName );
        if ( block )
            WaitForLoad();
    }
}

GTAScript::~GTAScript()
{
    if ( !m_bMarked )
        Unload();
}

void GTAScript::WaitForLoad()
{
    while ( !SCRIPT::HAS_SCRIPT_LOADED( m_szScriptName ) )
        WAIT( 0 );
}

bool GTAScript::IsValid()
{
    return m_bIsValid;
}

void GTAScript::Unload()
{
    if( IsValid() )
    {
        SCRIPT::SET_SCRIPT_AS_NO_LONGER_NEEDED( m_szScriptName );
        m_bMarked = true;
    }
}

const char *GTAScript::GetScriptName()
{
    return m_szScriptName;
}
