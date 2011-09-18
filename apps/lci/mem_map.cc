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
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "mem_map.hh"
#include "status.h"
#include "debug.h"

//------------------------------------------------------------------------------

mem_map::mem_map(uint32_t phys_base, size_t mem_size, int protocol, int flags)
{
  this->phys_base = phys_base;
  this->mem_size = mem_size;
  this->protocol = protocol;
  this->flags = flags;
  this->virtual_base = 0;
  this->fd = 0;
}

//------------------------------------------------------------------------------

mem_map::~mem_map()
{
  this->close();
}

//------------------------------------------------------------------------------

status_t mem_map::open()
{
  status_t status = FAILURE;

  this->fd = ::open("/dev/mem", O_RDWR | O_SYNC);
  if (this->fd < 0)
  {
    return FAILURE;
  }
  DPRINTF(DBG_LVL_CI, "mmap(0, %ld, %d, %d, %d, 0x%08X)\n", this->mem_size, PROT_READ | PROT_WRITE,
                                                 MAP_SHARED, this->fd, this->phys_base);
  this->virtual_base = (uint32_t) mmap((void *)0x0L,
                       this->mem_size, PROT_READ | PROT_WRITE,
                       MAP_SHARED, this->fd, this->phys_base);

  if (this->virtual_base != -1)
  {
    DPRINTF(DBG_LVL_CI, "virtual_base = 0x%08x\n", this->virtual_base);
    DPRINTF(DBG_LVL_CI, "mapped range = 0x%08X - 0x%08X\n", 
            this->phys_base, this->phys_base + this->mem_size - 1);
    status = SUCCESS;
  }
  else
  {
    DPRINTF(DBG_LVL_CI, "mem_map::open() failure\nerrno = %d\n", errno);
  }

  return status;
}

//------------------------------------------------------------------------------

void mem_map::print_map(void)
{
  printf("mapped range = 0x%08X - 0x%08X\n",
         this->phys_base, this->phys_base + this->mem_size - 1);
}

//------------------------------------------------------------------------------

void mem_map::close()
{
  if(this->fd > 0)
  {
    munmap((void*)this->virtual_base, this->mem_size);
    ::close(this->fd);
  }
}

//------------------------------------------------------------------------------

uint32_t mem_map::get_virtual_base()
{
  return this->virtual_base;
}

//------------------------------------------------------------------------------

uint32_t mem_map::get_virtual_addr(uint32_t real_addr)
{
  uint32_t virtual_addr =  this->virtual_base + (real_addr - this->phys_base);
  DPRINTF(DBG_LVL_CI, "vbase = 0x%08X, real_addr = 0x%08X, phys_base = 0x%08X, vaddr = 0x%08X\n",
          this->virtual_base, real_addr, this->phys_base, virtual_addr);
  return virtual_addr;
}

//------------------------------------------------------------------------------

bool mem_map::is_mapped(uint32_t real_addr)
{
  return ((real_addr >= this->phys_base) && 
          ((real_addr - this->phys_base) < this->mem_size));
}

//------------------------------------------------------------------------------

uint32_t mem_map::get_phys_base()
{
  return this->phys_base;
}

