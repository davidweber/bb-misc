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

#ifndef __STATUS_H__
#define __STATUS_H__

#define GENERAL_GROUP 100
#define CI_GROUP      200
#define API_HW_GROUP  300

typedef enum rt_status_e
{
  ABORTED = -2,
  FAILURE = -1,
  SUCCESS = 0,
  EXIT,

  MEMORY_ALLOCATION_FAILURE = GENERAL_GROUP,

  CI_UNRECOGNIZED_COMMAND = CI_GROUP,
  CI_INVALID_NUMBER_OF_ARGUMENTS,
  CI_INVALID_ARG,
  CI_INVALID_HW_REG_OBJECT,

  HW_INVALID_REGISTER_ACCESS_SIZE = API_HW_GROUP,
  HW_INVALID_ADDRESS,
  HW_INVALID_MEM_MAP_OBJECT,
  HW_CMD_MAP_FAILURE,
} status_t;

#endif // __STATUS_H__
