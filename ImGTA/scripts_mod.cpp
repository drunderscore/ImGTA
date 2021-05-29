/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "scripts_mod.h"
#include "natives.h"
#include <algorithm>
#include "imgui_extras.h"
#include "script.h"
#include "gta_script.h"

bool CompareScript( ScriptObject a, ScriptObject b )
{
    return a.m_handle < b.m_handle;
}

void ScriptsMod::Load()
{

}

void ScriptsMod::Unload()
{

}

void ScriptsMod::Think()
{
    if ( m_bWantsUpdate )
    {
        std::lock_guard<std::mutex> lock( m_scriptsMutex );
        m_bWantsUpdate = false;
        m_scripts.clear();
        SCRIPT::_BEGIN_ENUMERATING_THREADS();
        int id;
        while ( ( id = SCRIPT::_GET_ID_OF_NEXT_THREAD_IN_ENUMERATION() ) != 0 )
            m_scripts.push_back( ScriptObject( SCRIPT::_GET_NAME_OF_THREAD( id ), id ) );

        std::sort( m_scripts.begin(), m_scripts.end(), CompareScript );
    }
}

void ScriptsMod::ShowMenuBar()
{
    if ( ImGui::BeginMenuBar() )
    {
        if ( ImGui::BeginMenu( "Scripts" ) )
        {
            if ( ImGui::MenuItem( "Update" ) )
                m_bWantsUpdate = true;


            if ( ImGui::BeginMenu( "Start" ) )
            {
                ImGuiExtras::BitField( "Flags", &m_iStartFlags, nullptr );
                if ( ImGui::InputText( "Name", m_szStartScriptName, sizeof( m_szStartScriptName ), ImGuiInputTextFlags_EnterReturnsTrue ) )
                {
                    RunOnNativeThread( [ = ]
                    {
                        GTAScript script( m_szStartScriptName );
                        if ( script.IsValid() )
                            SYSTEM::START_NEW_SCRIPT( m_szStartScriptName, m_iStartFlags );

                        m_bWantsUpdate = true;
                    } );
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void ScriptsMod::ShowSelectedPopup()
{
    if ( ImGui::BeginPopup( "ScriptPropertiesPopup" ) )
    {
        if ( ImGui::MenuItem( "Terminate" ) )
        {
            RunOnNativeThread( [ = ]
            {
                SCRIPT::TERMINATE_THREAD( m_pSelected->m_handle );
                m_bWantsUpdate = true; // ask for the update only after this has actually been terminated.
            } );
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

bool ScriptsMod::Draw()
{
    ShowMenuBar();

    if ( m_scripts.size() > 0 )
    {
        ImGui::Columns( 2 );
        ImGui::Separator();
        ImGui::Text( "Name" ); ImGui::NextColumn();
        ImGui::Text( "Handle" ); ImGui::NextColumn();
        ImGui::Separator();

        std::lock_guard<std::mutex> lock( m_scriptsMutex );
        for ( auto &s : m_scripts )
        {
            if ( ImGui::Selectable( s.m_szName.c_str(), false, ImGuiSelectableFlags_SpanAllColumns ) )
            {
                m_pSelected = &s;
                ImGui::OpenPopup( "ScriptPropertiesPopup" );
            }
            ImGui::NextColumn();

            ImGui::Text( "%d (0x%x)", s.m_handle, s.m_handle ); ImGui::NextColumn();
        }
        ImGui::Columns( 1 );
        ImGui::Separator();
    }

    ShowSelectedPopup();

    return true;
}