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

#ifndef __CMD_TABLE_HH__
#define __CMD_TABLE_HH__

#include <stdint.h>
#include "status.h"

#define MAX_ARGS 8
#define MAX_ARG_LENGTH 20
#define CMD_BUF_LENGTH (MAX_ARGS * MAX_ARG_LENGTH + 1)
#define CMD_DESC_SIZE 64

typedef char  arg_t[MAX_ARG_LENGTH];
typedef arg_t arg_array_t[MAX_ARGS];

typedef status_t (*cmd_handler_t)(const char* line, arg_array_t, uint32_t);

typedef struct cmd_table_entry_s
{
  const char* cmd;
  cmd_handler_t cmd_handler;
  uint32_t min_args;
  uint32_t max_args;
  const char* desc;
} cmd_table_entry_t;

cmd_table_entry_t* cmd_table_get(void);
uint32_t cmd_table_get_num_entries(void);

#endif //__CMD_TABLE_H__

