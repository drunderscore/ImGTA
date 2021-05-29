/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "lua_console_mod.h"
#include "imgui_internal.h"
#include "script.h"
#include "lua_engine.h"
#include "lua.h"
#include <algorithm>

void LuaConsoleMod::Load()
{

}

void LuaConsoleMod::Unload()
{

}

void LuaConsoleMod::Think()
{

}

void LuaConsoleMod::ShowMenuBar()
{
    if ( ImGui::BeginMenuBar() )
    {
        if ( ImGui::BeginMenu( "Lua" ) )
        {
            if ( ImGui::MenuItem( "Reload natives.lua" ) )
                LoadLuaNatives();

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

bool LuaConsoleMod::Draw()
{
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // thanks omar

    ShowMenuBar();

    if ( ImGui::BeginChild( "ConsoleText", ImVec2( 0.0f, -footer_height_to_reserve ) ) )
        ImGui::TextUnformatted( lua_stream.str().c_str() );
    ImGui::EndChild(); // legacy dont-care-about-return-value end.

    ImGui::Separator();
    ImGui::SetNextItemWidth( ImGui::GetCurrentWindow()->Size.x - 75.0f );

    //bool uparrow = ImGui::IsKeyPressedMap( ImGuiKey_UpArrow, false );
    //bool downarrow = ImGui::IsKeyPressedMap( ImGuiKey_DownArrow, false );

    //if ( uparrow )
    //    m_iCurrentHistory = std::min( m_iCurrentHistory + 1, ( int )m_commandHistory.size() - 1 );
    //else if ( downarrow )
    //    m_iCurrentHistory = std::max( m_iCurrentHistory - 1, 0 );

    //if ( m_commandHistory.size() > 0 && ( uparrow || downarrow ) )
    //    strcpy_s( m_szInput, m_commandHistory[m_commandHistory.size() - m_iCurrentHistory].c_str() );

    if ( ImGui::InputText( "Input", m_szInput, sizeof( m_szInput ), ImGuiInputTextFlags_EnterReturnsTrue ) )
    {
        strcpy_s( m_szLuaInput, m_szInput );
        strcpy_s( m_szInput, "" );
        m_commandHistory.push_back( m_szLuaInput );
        m_iCurrentHistory = 0;
        ImGui::SetKeyboardFocusHere();

        lua_stream << "> " << m_szLuaInput << std::endl;
        RunOnNativeThread( [ = ]
        {
            if ( DoLuaString( m_szLuaInput ) != LUA_OK )
                lua_stream << GetLastLuaError() << std::endl;
        } );
    }

    return true;
}
