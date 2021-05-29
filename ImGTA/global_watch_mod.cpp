/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "global_watch_mod.h"
#include "imgui.h"
#include "main.h"
#include "utils.h"
#include "watch_entry.h"
#include "imgui_extras.h"

#include <vector>
#include <stdio.h>
#include <inttypes.h>
#include <mutex>
#include <algorithm>
#include <bitset>

const char *watchTypeNames[] = { "Int", "Float", "String", "Vector", "Bitfield" };

void GlobalWatchMod::Load()
{

}

void GlobalWatchMod::Unload()
{

}

void GlobalWatchMod::Think()
{
    if ( m_watches.size() > 0 )
    {
        std::lock_guard<std::mutex> lock( m_watchesMutex );

        char buf[128] = "";
        float yOff = 0.08f;
        for ( auto &w : m_watches )
        {
            if ( w.m_bDrawIngame )
            {
                sprintf_s( buf, "0x%x: %s", w.m_iGlobalAddress, GetDisplayForType( getGlobalPtr( w.m_iGlobalAddress ), w.m_type ).c_str() );
                DrawTextToScreen( buf, 0.05f, yOff, 0.3f, eFont::FontChaletLondon );
                yOff += 0.02f;
            }
        }
    }
}

void GlobalWatchMod::ShowAddAddress()
{
    ImGui::InputInt( "Address##AddAddress", &m_iInputAddr, 1, 100, ImGuiInputTextFlags_CharsHexadecimal );
    ImGui::Combo( "Type##AddAddress", &m_iInputType, watchTypeNames, IM_ARRAYSIZE( watchTypeNames ) );
    if ( ImGui::Button( "Add##AddAddress" ) )
    {
        if ( getGlobalPtr( m_iInputAddr ) == nullptr )
            return;

        std::lock_guard<std::mutex> lock( m_watchesMutex );
        m_watches.push_back( WatchEntry( m_iInputAddr, ( WatchType )m_iInputType ) );
    }
}

void GlobalWatchMod::ShowSelectedPopup()
{
    if ( ImGui::BeginPopup( "PopupEntryProperties" ) )
    {
        ImGui::Combo( "Type##EntryProperties", ( int * )&m_pSelectedEntry->m_type, watchTypeNames, IM_ARRAYSIZE( watchTypeNames ) );
        ImGui::Checkbox( "Show Ingame##EntryProperties", &m_pSelectedEntry->m_bDrawIngame );

        uint64_t *val = getGlobalPtr( m_pSelectedEntry->m_iGlobalAddress );

        switch ( m_pSelectedEntry->m_type )
        {
            case WatchType::kBitfield:
                ImGuiExtras::BitField( "Value##GlobalWatchValueBitfield", ( unsigned int * )val, nullptr );
                if ( ImGui::Button( "LS<<##GlobalWatchLBitshift" ) )
                    *val = *val << 1;
                if ( ImGui::Button( ">>RS##GlobalWatchRBitshift" ) )
                    *val = *val >> 1;

            case WatchType::kInt:
                ImGui::InputInt( "Value##GlobalWatchValue", ( int * )val );
                break;
            case WatchType::kFloat:
                ImGui::InputFloat( "Value##GlobalWatchValue", ( float * )val, 0.0f, 0.0f, "%.4f" );
                break;
            case WatchType::kVector:
                ImGuiExtras::InputVector3( "GlobalWatchValue", ( Vector3 * )val );
                break;
            case WatchType::kString:
                ImGui::TextDisabled( "Cannot edit string." );
                break;
        }

        if ( ImGui::Button( "Remove##EntryProperties" ) )
        {
            std::lock_guard<std::mutex> lock( m_watchesMutex );
            m_watches.erase( std::remove( m_watches.begin(), m_watches.end(), m_pSelectedEntry ), m_watches.end() );
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void GlobalWatchMod::ShowMenuBar()
{
    if ( ImGui::BeginMenuBar() )
    {
        if ( ImGui::BeginMenu( "Watch" ) )
        {
            if ( ImGui::BeginMenu( "Add Address" ) )
            {
                ShowAddAddress();
                ImGui::EndMenu();
            }

            if ( ImGui::MenuItem( "Clear" ) )
            {
                std::lock_guard<std::mutex> lock( m_watchesMutex );
                m_watches.clear();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

bool GlobalWatchMod::Draw()
{
    ShowMenuBar();
    ImGui::TextColored( ImVec4( 255, 0, 0, 255 ), "Game version: %d", getGameVersion() );

    char buf[128] = "";

    ImGui::Columns( 3 );
    ImGui::Separator();
    ImGui::Text( "Address" ); ImGui::NextColumn();
    ImGui::Text( "Type" ); ImGui::NextColumn();
    ImGui::Text( "Value" ); ImGui::NextColumn();
    ImGui::Separator();
    if ( m_watches.size() > 0 )
    {
        std::lock_guard<std::mutex> lock( m_watchesMutex );
        for ( auto &w : m_watches )
        {
            uint64_t *addr = getGlobalPtr( w.m_iGlobalAddress );

            sprintf_s( buf, "0x%x", w.m_iGlobalAddress );

            if ( ImGui::Selectable( buf, false, ImGuiSelectableFlags_SpanAllColumns ) )
            {
                m_pSelectedEntry = &w;
                ImGui::OpenPopup( "PopupEntryProperties" );
            }

            ImGui::NextColumn();
            ImGui::Text( "%s", watchTypeNames[w.m_type] ); ImGui::NextColumn();
            ImGui::Text( "%s", GetDisplayForType( addr, w.m_type ).c_str() ); ImGui::NextColumn();
        }
        ImGui::Columns( 1 );
        ImGui::Separator();
    }

    ShowSelectedPopup();
    return true;
}