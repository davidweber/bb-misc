/*
 * Copyright (C) 2004-2011 David Weber <david@embeddedtoolbox.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DBG_LVL_ERROR 0
#define DBG_LVL_CI    4

void debug_set_debug_level(int debug_level);
int debug_get_debug_level(void);

#ifdef DEBUG_ON
 #include <stdio.h>
 #define DPRINTF(debug_level, ...) if (debug_get_debug_level() >= debug_level) printf(__VA_ARGS__)
 #define DEBUG_FUNCTION_ENTER(debug_level) DPRINTF(debug_level, "%s() entered\n", __FUNCTION__)
 #define DEBUG_FUNCTION_EXIT(debug_level) DPRINTF(debug_level, "%s() exited\n", __FUNCTION__)
#else
 #define DPRINTF(...)
 #define DEBUG_FUNCTION_ENTER(...)
 #define DEBUG_FUNCTION_EXIT(...)
#endif

#endif // _DEBUG_H_

