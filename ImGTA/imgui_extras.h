/*
 * Copyright (c) 2021, James Puleo <james@jame.xyz>
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

namespace ImGuiExtras
{
    bool BitField( const char *label, unsigned *bits, unsigned *hoverIndex );
    bool InputVector3( const char *label, Vector3 *vec, const char *fmt = "%.3f", ImGuiInputTextFlags flags = 0 );
}