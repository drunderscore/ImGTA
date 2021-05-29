/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include <sstream>

void InitLuaEngine();
void UnloadLuaEngine();
int DoLuaString( const char *line );
int DoLuaFile( const char *path );
bool IsLuaReady();
const char *GetLastLuaError();
void LoadLuaNatives();

extern std::ostringstream lua_stream;