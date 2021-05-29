/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "types.h"

class Model
{
public:
    Model( const char *model, bool block = true );
    Model( Hash model, bool block = true );
    ~Model();

    void Unload();
    Hash GetHash();
    bool IsValid();
    void WaitForLoad();
    bool IsVehicle();

private:
    bool m_bIsValid = false;
    bool m_bMarked = false;
    Hash m_hash = 0;
};