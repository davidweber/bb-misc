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

#include "ci.hh"
#include "mem_map.hh"
#include "hw_reg.hh"
#include "status.h"

#ifndef __CMD_H__
#define __CMD_H__

//!-----------------------------------------------------------------------------
//! This class contains the implementation of all supported commands
//!-----------------------------------------------------------------------------
class cmd
{
  private:
  
  protected:

  public:

  static mem_map* mem_map_p;
  static hw_reg* hw_reg_p;

  //! Constructor declaration for cmd class
  cmd();

  //! Destructor declaration for cmd class
  ~cmd(); 

  static status_t map(uint32_t base);
  static status_t help(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t exit(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t quit(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t r8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t r16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t r32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t w8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t w16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t w32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t s8 (const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t s16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t s32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t c8 (const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t c16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t c32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t source(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t mf8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t mf16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t mf32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t ms8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t ms16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t ms32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t mcpy(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t mcmp(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t timestamp(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t version(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t debug(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t pause(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t sleep(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t cd(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t lcd(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t i2c_read(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
  static status_t i2c_write(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
#if 0
  static status_t hello(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args);
#endif
};

#endif //__CMD_H__
