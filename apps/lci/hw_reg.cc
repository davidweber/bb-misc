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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "hw_reg.hh" 
#include "mem_map.hh"
#include "status.h"

//------------------------------------------------------------------------------

hw_reg::hw_reg(mem_map* mem_map_p)
{
  this->mem_map_p = mem_map_p;
}

//------------------------------------------------------------------------------

hw_reg::~hw_reg()
{
}

//------------------------------------------------------------------------------

status_t hw_reg::r8(const uint32_t addr, uint8_t* data)
{
  status_t status = SUCCESS;

  if (!mem_map_p->is_mapped(addr))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *data = *(volatile uint8_t*)mem_map_p->get_virtual_addr(addr);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t hw_reg::r16(const uint32_t addr, uint16_t* data)
{
  status_t status = SUCCESS;

  if ((!mem_map_p->is_mapped(addr)) || (addr & 1))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *data = *(volatile uint16_t*)mem_map_p->get_virtual_addr(addr);
  }

  return status;
}

//------------------------------------------------------------------------------

status_t hw_reg::r32(const uint32_t addr, uint32_t* data)
{
  status_t status = SUCCESS; 

  if ((!mem_map_p->is_mapped(addr)) || (addr & 3))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *data = *(volatile uint32_t*)mem_map_p->get_virtual_addr(addr);
  }

  return status;
}


//------------------------------------------------------------------------------

status_t hw_reg::w8(const uint32_t addr, const uint8_t data)
{
  status_t status = SUCCESS;

  if (!mem_map_p->is_mapped(addr))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *(volatile uint8_t*)mem_map_p->get_virtual_addr(addr) = data;
  }

  return status;
}

//------------------------------------------------------------------------------

status_t hw_reg::w16(const uint32_t addr, const uint16_t data)
{
  status_t status = SUCCESS;
 
  if ((!mem_map_p->is_mapped(addr)) || (addr & 1))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *(volatile uint16_t*)mem_map_p->get_virtual_addr(addr) = data;
  }
  
  return status;
}

//------------------------------------------------------------------------------

status_t hw_reg::w32(const uint32_t addr, const uint32_t data)
{
  status_t status = SUCCESS;

  if ((!mem_map_p->is_mapped(addr)) || (addr & 3))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *(volatile uint32_t*)mem_map_p->get_virtual_addr(addr) = data;
  }

  return status; 
}

//------------------------------------------------------------------------------

status_t hw_reg::s8(const uint32_t addr, const uint8_t data)
{
  status_t status = SUCCESS;

  if (!mem_map_p->is_mapped(addr))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *(volatile uint8_t*)mem_map_p->get_virtual_addr(addr) |= data;
  }

  return status;
}

//------------------------------------------------------------------------------

status_t hw_reg::s16(const uint32_t addr, const uint16_t data)
{
  status_t status = SUCCESS;
 
  if ((!mem_map_p->is_mapped(addr)) || (addr & 1))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *(volatile uint16_t*)mem_map_p->get_virtual_addr(addr) |= data;
  }
  
  return status;
}

//------------------------------------------------------------------------------

status_t hw_reg::s32(const uint32_t addr, const uint32_t data)
{
  status_t status = SUCCESS;

  if ((!mem_map_p->is_mapped(addr)) || (addr & 3))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *(volatile uint32_t*)mem_map_p->get_virtual_addr(addr) |= data;
  }

  return status; 
}

//------------------------------------------------------------------------------

status_t hw_reg::c8(const uint32_t addr, const uint8_t data)
{
  status_t status = SUCCESS;

  if (!mem_map_p->is_mapped(addr))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *(volatile uint8_t*)mem_map_p->get_virtual_addr(addr) &= (~data);
  }

  return status;
}

//------------------------------------------------------------------------------

status_t hw_reg::c16(const uint32_t addr, const uint16_t data)
{
  status_t status = SUCCESS;
 
  if ((!mem_map_p->is_mapped(addr)) || (addr & 1))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *(volatile uint16_t*)mem_map_p->get_virtual_addr(addr) &= (~data);
  }
  
  return status;
}

//------------------------------------------------------------------------------

status_t hw_reg::c32(const uint32_t addr, const uint32_t data)
{
  status_t status = SUCCESS;

  if ((!mem_map_p->is_mapped(addr)) || (addr & 3))
  {
    status = HW_INVALID_ADDRESS;
  }
  else
  {
    *(volatile uint32_t*)mem_map_p->get_virtual_addr(addr) &= (~data);
  }

  return status; 
}

