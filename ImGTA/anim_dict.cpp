/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "anim_dict.h"
#include "main.h"
#include "natives.h"

AnimDict::AnimDict( const char *dict, bool block )
{
    strcpy_s( m_szAnimDict, dict );

    m_bIsValid = STREAMING::DOES_ANIM_DICT_EXIST( ( char * )dict );

    if ( IsValid() )
    {
        STREAMING::REQUEST_ANIM_DICT( ( char * )dict );
        if ( block )
            WaitForLoad();
    }
}

AnimDict::~AnimDict()
{
    //if ( !m_bMarked )
    //    Unload();
}

void AnimDict::WaitForLoad()
{
    while ( !STREAMING::HAS_ANIM_DICT_LOADED( m_szAnimDict ) )
        WAIT( 0 );
}

bool AnimDict::IsValid()
{
    return m_bIsValid;
}

void AnimDict::Unload()
{
    if ( m_bIsValid )
    {
        STREAMING::REMOVE_ANIM_DICT( m_szAnimDict );
        m_bMarked = true;
    }
}

const char *AnimDict::GetDict()
{
    return m_szAnimDict;
}