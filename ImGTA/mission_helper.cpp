/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "mission_helper.h"
#include <stdexcept>
#include <map>

bool MissionHelper::IsVersionSupported( eGameVersion ver )
{
    return ver == eGameVersion::VER_1_0_372_2_STEAM; // TODO: are globals the same in steam and nonsteam?
}

MissionHelper::MissionHelper( eGameVersion ver ) : m_version( ver )
{
    if ( !IsVersionSupported( ver ) )
        throw new std::logic_error( "Unsupported version supplied for MissionHelper" );
}

//void MissionHelper::Fail()
//{
//    *getGlobalPtr( 0x15F6A ) = 0;
//}

void MissionHelper::Skip()
{
    *getGlobalPtr( 0x14A40 ) = 1;
    *getGlobalPtr( 0xD5DC ) = 1;
    *getGlobalPtr( 0x15F6A ) = 9;
}