/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "utils.h"
#include "natives.h"
#include "enums.h"
#include "mod.h"

BOOL is_main_window( HWND handle )
{
    return GetWindow( handle, GW_OWNER ) == ( HWND )0 && IsWindowVisible( handle );
}

BOOL CALLBACK enum_windows_callback( HWND handle, LPARAM lParam )
{
    handle_data &data = *( handle_data * )lParam;
    unsigned long process_id = 0;
    GetWindowThreadProcessId( handle, &process_id );
    if ( data.process_id != process_id || !is_main_window( handle ) )
        return TRUE;
    data.window_handle = handle;
    return FALSE;
}

// https://stackoverflow.com/questions/1888863/how-to-get-main-window-handle-from-process-id
HWND find_main_window( unsigned long process_id )
{
    handle_data data;
    data.process_id = process_id;
    data.window_handle = 0;
    EnumWindows( enum_windows_callback, ( LPARAM )&data );
    return data.window_handle;
}

void DrawTextToScreen( const char *text, float x, float y, float scale, eFont font )
{
    UI::SET_TEXT_FONT( ( int )font );
    UI::SET_TEXT_SCALE( scale, scale );
    UI::SET_TEXT_COLOUR( 255, 255, 255, 255 );
    UI::SET_TEXT_WRAP( 0.0, 1.0 );
    UI::SET_TEXT_CENTRE( false );
    UI::SET_TEXT_DROPSHADOW( 2, 2, 0, 0, 0 );
    UI::SET_TEXT_EDGE( 1, 0, 0, 0, 205 );
    UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT( ( char * )"STRING" );
    UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME( ( char * )text );
    UI::END_TEXT_COMMAND_DISPLAY_TEXT( x, y );
}