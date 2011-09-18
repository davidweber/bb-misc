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

#include <stdlib.h>
#include "ci.hh"
#include "cmd_table.h"
#include "cmd.hh"
#include "debug.h"

ci* ci_p = NULL;

int main ( int argc, char* argv[] )
{
  if (argc == 2)
  {
    debug_set_debug_level(atoi(argv[1]));
  }
  char hostname[10];

  const char* prompt = PROMPT;
  cmd_table_entry_t* cmd_table = cmd_table_get();
  uint32_t num_entries = cmd_table_get_num_entries();
  ci_p = new ci (cmd_table, num_entries, prompt);
  ci_p->run();
  delete(ci_p);
}

