/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "mod.h"
#include "watch_entry.h"
#include <vector>
#include <mutex>

class GlobalWatchMod : public Mod
{
public:
    GlobalWatchMod() : Mod( "Global Watch", true )
    {
        m_iWindowFlags = ImGuiWindowFlags_MenuBar;
    }

    bool Draw() override;
    void Think() override;
    void Load() override;
    void Unload() override;

private:
    void ShowAddAddress();
    void ShowMenuBar();
    void ShowSelectedPopup();

    std::vector<WatchEntry> m_watches;
    std::mutex m_watchesMutex;

    WatchEntry *m_pSelectedEntry = nullptr;
    int m_iInputAddr = 0;
    int m_iInputType = 0;
};