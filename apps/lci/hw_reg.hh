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

#ifndef __HW_REG_H__
#define __HW_REG_H__

#include <stdint.h>
#include <stdbool.h>
#include "status.h"
#include "mem_map.hh"

class hw_reg
{
  private:

  bool opened;
  mem_map* mem_map_p;

  protected:

  public:

  hw_reg(mem_map* mem_map_p);

  ~hw_reg(); 

  status_t r8(const uint32_t addr, uint8_t* data);
  status_t r16(const uint32_t addr, uint16_t* data);
  status_t r32(const uint32_t addr, uint32_t* data);
  status_t w8(const uint32_t addr, const uint8_t data);
  status_t w16(const uint32_t addr, const uint16_t data);
  status_t w32(const uint32_t addr, const uint32_t data);
  status_t s8(const uint32_t addr, const uint8_t data);
  status_t s16(const uint32_t addr, const uint16_t data);
  status_t s32(const uint32_t addr, const uint32_t data);
  status_t c8(const uint32_t addr, const uint8_t data);
  status_t c16(const uint32_t addr, const uint16_t data);
  status_t c32(const uint32_t addr, const uint32_t data);
  mem_map* get_mem_map(void) { return mem_map_p; }
};

#endif //__HW_REG_H__
