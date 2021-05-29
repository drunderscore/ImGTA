/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include <functional>

struct IDXGISwapChain;

void Load();
void Unload();
void OnPresent( IDXGISwapChain *swap );

// Relying on this function is not recommended!
void RunOnNativeThread( std::function<void()> func );