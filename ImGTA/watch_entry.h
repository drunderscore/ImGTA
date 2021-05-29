/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include <string>

enum WatchType
{
    kInt,
    kFloat,
    kString,
    kVector,
    kBitfield
};

class WatchEntry
{
public:
    WatchEntry( int addr, WatchType type ) : m_iGlobalAddress( addr ), m_type( type ), m_bDrawIngame( false ) {}
    int m_iGlobalAddress;
    WatchType m_type;
    bool m_bDrawIngame;

    bool operator==( WatchEntry *other )
    {
        return this->m_iGlobalAddress == other->m_iGlobalAddress;
    }
};


std::string GetDisplayForType( uint64_t *addr, WatchType type );