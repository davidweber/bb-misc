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

#include <stdint.h>
#include "cmd.hh"
#include "cmd_table.h"

//------------------------------------------------------------------------------

cmd_table_entry_t cmd_table[] =
{
  { "C16",      cmd::c16,          3, 3, "C16     <addr> <data>       - clears specified bits (read-modify-write)" },
  { "C32",      cmd::c32,          3, 3, "C32     <addr> <data>       - clears specified bits (read-modify-write)" },
  { "C8",       cmd::c8,           3, 3, "C8      <addr> <data>       - clears specified bits (read-modify-write)" },
  { "CD",       cmd::cd,           1, 2, "CD      [path]              - changes current directory to [path]" },
  { "DEBUG",    cmd::debug,        1, 2, "DEBUG   [debug level]       - gets or sets the debug level 0 = none" },
  { "EXIT",     cmd::exit,         1, 1, "EXIT                        - exits program" },
  { "HELP",     cmd::help,         1, 1, "HELP                        - displays this command summary" },
  { "MCMP",     cmd::mcmp,         4, 4, "MCMP    <dest> <src> <sz>   - compares sz bytes between dest and src memory" },
  { "MCPY",     cmd::mcpy,         4, 4, "MCPY    <dest> <src> <sz>   - copies sz bytes from src to dest" },
  { "MF16",     cmd::mf16,         4, 4, "MF16    <addr> <n> <seed>   - fills n addresses with incremented seed values" },
  { "MF32",     cmd::mf32,         4, 4, "MF32    <addr> <n> <seed>   - fills n addresses with incremented seed values" },
  { "MF8",      cmd::mf8,          4, 4, "MF8     <addr> <n> <seed>   - fills n addresses with incremented seed values" },
  { "MS16",     cmd::ms16,         4, 4, "MS16    <addr> <n> <data>   - fills n addresses with specified 16-bit data value" },
  { "MS32",     cmd::ms32,         4, 4, "MS32    <addr> <n> <data>   - fills n addresses with specified 32-bit data value" },
  { "MS8",      cmd::ms8,          4, 4, "MS8     <addr> <n> <data>   - fills n addresses with specified 8-bit data value" },
  { "QUIT",     cmd::exit,         1, 1, "QUIT                        - exits program" },
  { "PAUSE",    cmd::pause,        0, 0, "PAUSE   <msg>               - displays msg and waits for user to hit enter" },
  { "R16",      cmd::r16,          2, 3, "R16     <addr> [n]          - read 16-bit value" },
  { "R32",      cmd::r32,          2, 3, "R32     <addr> [n]          - read 32-bit value" },
  { "R8",       cmd::r8,           2, 3, "R8      <addr> [n]          - read 8-bit value" },
  { "S16",      cmd::s16,          3, 3, "S16     <addr> <data>       - sets specified bits (read-modify-write)" },
  { "S32",      cmd::s32,          3, 3, "S32     <addr> <data>       - sets specified bits (read-modify-write)" },
  { "S8",       cmd::s8,           3, 3, "S8      <addr> <data>       - sets specified bits (read-modify-write)" },
  { "SLEEP",    cmd::sleep,        2, 2, "SLEEP   <seconds>           - sleeps for specified seconds" },
  { "SOURCE",   cmd::source,       2, 2, "SOURCE  <filename>          - executes command from specified file" },
  { "VER",      cmd::version,      1, 1, "VER                         - displays program version"},
  { "W16",      cmd::w16,          3, 3, "W16     <addr> <data>       - write 16-bit value" },
  { "W32",      cmd::w32,          3, 3, "W32     <addr> <data>       - write 32-bit value" },
  { "W8",       cmd::w8,           3, 3, "W8      <addr> <data>       - write 8-bit value" },
#if 1
  { "LCD"      ,cmd::lcd,          2, 2, "LCD <cmd>                   - do lcd cmd <cmd> = on, off, flip, toggle" },
  { "I2CRD"    ,cmd::i2c_read,     2, 2, "I2C_RD  <addr>              - read I2C addr" },
  { "I2CWR"    ,cmd::i2c_write,    3, 3, "I2C_WR  <addr> <data>       - write specifed data to specified I2C addr" },
#endif
};

//------------------------------------------------------------------------------

uint32_t cmd_table_get_num_entries(void)
{
  return (sizeof(cmd_table) / sizeof(cmd_table_entry_t));
}

//------------------------------------------------------------------------------

cmd_table_entry_t* cmd_table_get(void)
{
  return cmd_table;
}
