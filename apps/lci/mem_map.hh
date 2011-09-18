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

#ifndef __MEM_MAP_H__
#define __MEM_MAP_H__

#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "status.h"

#define  MEM_SIZE 0x10000000 // 256MB

class mem_map
{
  private:
    int fd;
    uint32_t phys_base;
    uint32_t virtual_base;
    size_t mem_size;
    int protocol;
    int flags;

  public:
    mem_map(uint32_t phys_base, size_t mem_size = MEM_SIZE,
            int protocol = PROT_READ | PROT_WRITE,
            int flags = MAP_SHARED);
    ~mem_map();
    status_t open();
    void close();
    uint32_t get_virtual_base();
    uint32_t get_virtual_addr(uint32_t real_addr);
    uint32_t get_phys_base();
    bool     is_mapped(uint32_t real_addr);
    void     print_map(void);
};

#endif // __MEM_MAP_H__

