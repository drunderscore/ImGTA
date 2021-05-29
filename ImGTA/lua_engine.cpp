/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "lua_engine.h"
#include "lua.hpp"
#include "nativeCaller.h"

lua_State *state;
std::ostringstream lua_stream;

// TODO: can we combine these somehow?
void lua_BaseInvoke( lua_State *L )
{
    UINT64 addr = luaL_checkinteger( L, 1 );
    nativeInit( addr );

    for ( int i = 2; i <= lua_gettop( L ); i++ )
    {
        if ( lua_isinteger( L, i ) )
            nativePush<int>( lua_tointeger( L, i ) );
        else if ( lua_isnumber( L, i ) )
            nativePush( static_cast<float>( lua_tonumber( L, i ) ) );
        else if ( lua_isstring( L, i ) )
            nativePush( lua_tostring( L, i ) );
        else
            luaL_error( L, "not an integer, number, or string" );
        lua_pop( L, 1 );
    }

}

int lua_Invoke( lua_State *L )
{
    lua_BaseInvoke( L );

    lua_pushnumber( L, *nativeCall() );

    return 1;
}

int lua_InvokeString( lua_State *L )
{
    lua_BaseInvoke( L );

    lua_pushstring( L, ( char * )*nativeCall() );

    return 1;
}

int lua_InvokeInteger( lua_State *L )
{
    lua_BaseInvoke( L );

    lua_pushinteger( L, static_cast<int>( *nativeCall() ) );

    return 1;
}

static int lua_Print( lua_State *L )
{
    int n = lua_gettop( L );  /* number of arguments */
    int i;
    for ( i = 1; i <= n; i++ )
    {
        /* for each argument */
        const char *s = luaL_tolstring( L, i, nullptr );  /* convert it to string */
        if ( i > 1 )  /* not the first element? */
            lua_stream << "\t";  /* add a tab before it */
        lua_stream << s;
        lua_pop( L, 1 );  /* pop result */
    }
    lua_stream << std::endl;
    return 0;
}

static int lua_GetGlobal( lua_State *L )
{
    int addr = static_cast<int>( luaL_checkinteger( L, 1 ) );

    if ( addr < 0 )
        luaL_error( L, "invalid global address" );

    UINT64 *realAddr = getGlobalPtr( addr );

    if ( !realAddr )
        luaL_error( L, "invalid global address" );

    lua_pushnumber( L, *realAddr );

    return 1;
}

static int lua_SetGlobal( lua_State *L )
{
    int addr = luaL_checkinteger( L, 1 );

    if ( addr < 0 )
        luaL_error( L, "invalid global address" );

    UINT64 *realAddr = getGlobalPtr( addr );

    if ( !realAddr )
        luaL_error( L, "invalid global address" );

    if ( lua_isinteger( L, 2 ) )
        *realAddr = lua_tointeger( L, 2 );
    else if ( lua_isnumber( L, 2 ) )
        *( float * )realAddr = static_cast<float>( lua_tonumber( L, 2 ) );
    else // TOOD: allow string copy. (dangerous, technically)
        luaL_error( L, "not an integer or number." );

    lua_pop( L, 1 );

    return 0;
}

static const struct luaL_Reg baselib[] =
{
    { "print", lua_Print }, // override print with our print
    { NULL, NULL }
};

static const struct luaL_Reg gtalib[] =
{
    { "invoke", lua_Invoke },
    { "invokeString", lua_InvokeString },
    { "invokeInteger", lua_InvokeInteger },
    { "getGlobal", lua_GetGlobal },
    { "setGlobal", lua_SetGlobal },
    { NULL, NULL }
};

void LoadLuaNatives()
{
    if ( luaL_dofile( state, "natives.lua" ) != LUA_OK )
    {
        lua_stream << "WARN: Unable to dofile for natives.lua!" << std::endl;
        lua_stream << "\t" << GetLastLuaError() << std::endl;
    }
}

void InitLuaEngine()
{
    if ( state )
        return;

    state = luaL_newstate();
    luaL_openlibs( state );
    lua_pushglobaltable( state );
    luaL_setfuncs( state, baselib, 0 );
    luaL_setfuncs( state, gtalib, 0 );
    LoadLuaNatives();
}

void UnloadLuaEngine()
{
    if ( !state )
        return;
    lua_close( state );
}

int DoLuaString( const char *line )
{
    return luaL_dostring( state, line );
}

int DoLuaFile( const char *path )
{
    return luaL_dofile( state, path );
}

bool IsLuaReady()
{
    return state != nullptr;
}

const char *GetLastLuaError()
{
    return lua_tostring( state, -1 );
}