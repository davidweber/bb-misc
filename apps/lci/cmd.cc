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
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "cmd.hh" 
#include "ci.hh"
#include "cmd_table.h"
#include "mem_map.hh"
#include "hw_reg.hh"
#include "status.h"
#include "debug.h"

mem_map* cmd::mem_map_p = NULL;
hw_reg* cmd::hw_reg_p = NULL;

volatile bool g_done = false;

//------------------------------------------------------------------------------

static void sig_handler(int sig)
{
  if (sig == SIGINT)
  {
    g_done = true;
    printf("aborted\n");
  }
}

//------------------------------------------------------------------------------

cmd::cmd()
{
}

//------------------------------------------------------------------------------

cmd::~cmd()
{
}

//------------------------------------------------------------------------------

status_t cmd::map(uint32_t base)
{
  status_t status = SUCCESS;

//  signal(SIGINT, sig_handler);

  if (mem_map_p != NULL) 
  {
    if (mem_map_p->is_mapped(base) != true)
    {
      delete mem_map_p;
      mem_map_p = new mem_map(base);

      if (mem_map_p == NULL)
      {
        status = MEMORY_ALLOCATION_FAILURE;
      }
      else
      {
        status = mem_map_p->open();
      }
    }
  }
  else
  {
    mem_map_p = new mem_map(base);

    if (mem_map_p == NULL)
    {
      status = MEMORY_ALLOCATION_FAILURE;
    }
    else
    {
      status = mem_map_p->open();
    }
  }
  
  if (status == SUCCESS)
  {
    if (hw_reg_p != NULL)
    {
      delete hw_reg_p;
    }

    hw_reg_p = new hw_reg(mem_map_p);

    if (hw_reg_p == NULL)
    {
      status = MEMORY_ALLOCATION_FAILURE;
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::help(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t i;
  cmd_table_entry_t* cmd_table = cmd_table_get();
  printf("\n");
  printf("  Command Summary\n");
  printf("--------------------------------------------------------------------------------------------\n");
  printf("\n");
  for(i = 0; i < cmd_table_get_num_entries(); i++)
  {
    printf("  %s\n", cmd_table[i].desc);
  }
  printf("\n");
  printf("--------------------------------------------------------------------------------------------\n");
  return SUCCESS;
}

//------------------------------------------------------------------------------

status_t cmd::exit(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  return EXIT;
}

//------------------------------------------------------------------------------

status_t cmd::quit(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  return EXIT;
}

//------------------------------------------------------------------------------

status_t cmd::cd(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  char* pwd;

  if (num_args == 1)
  {
    pwd = getenv("HOME");
    chdir(pwd);
  }
  else if (num_args == 2)
  {
    chdir(cmd_args[1]);
  //  setenv("PWD", get_current_dir_name(), true);
  }
}

//------------------------------------------------------------------------------

status_t cmd::r8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = CI_INVALID_NUMBER_OF_ARGUMENTS;

  if (num_args == 2)
  {
    uint32_t addr;
    uint8_t  data;
    if(sscanf(cmd_args[1], "%x", &addr) != 1)
    {
      status = CI_INVALID_ARG;
    }
    else if (map(addr & 0xF0000000) != SUCCESS)
    {
      status = HW_CMD_MAP_FAILURE;
    }
    else
    {
      status = hw_reg_p->r8(addr, &data);
      if (status == SUCCESS)
      {
        printf("[0x%08X] = 0x%02X  %d\n", addr, data, data);
      }
    }
  }
  else if (num_args == 3)
  {
    uint32_t addr;
    uint32_t n;
    uint8_t data;
    if((sscanf(cmd_args[1], "%x", &addr) != 1) ||
       (sscanf(cmd_args[2], "%d", &n) != 1))
    {
      status = CI_INVALID_ARG;
    }
    else if (map(addr & 0xF0000000) != SUCCESS)
    {
      status = HW_CMD_MAP_FAILURE;
    }
    else
    {
      for (uint32_t i = 0; i < n; i++)
      {
        status = hw_reg_p->r8(addr + i, &data);
        if (status == SUCCESS)
        {
          printf("[0x%08X] = 0x%02X  %d\n", addr + i, data, data);
        }
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::r16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = CI_INVALID_NUMBER_OF_ARGUMENTS;

  if (num_args == 2)
  {
    uint32_t addr;
    uint16_t data;
    if(sscanf(cmd_args[1], "%x", &addr) != 1)
    {
      status = CI_INVALID_ARG;
    }
    else if (map(addr & 0xF0000000) != SUCCESS)
    {
      status = HW_CMD_MAP_FAILURE;
    }
    else
    {
      status = hw_reg_p->r16(addr, &data);
      if (status == SUCCESS)
      {
        printf("[0x%08X] = 0x%04X  %d\n", addr, data, data);
      }
    }
  }
  else if (num_args == 3)
  {
    uint32_t addr;
    uint32_t n;
    uint16_t data;
    if((sscanf(cmd_args[1], "%x", &addr) != 1) ||
       (sscanf(cmd_args[2], "%d", &n) != 1))
    {
      status = CI_INVALID_ARG;
    }
    else if (map(addr & 0xF0000000) != SUCCESS)
    {
      status = HW_CMD_MAP_FAILURE;
    }
    else
    {
      for (uint32_t i = 0; i < n; i++)
      {
        status = hw_reg_p->r16(addr + (i << 1), &data);
        if (status == SUCCESS)
        {
          printf("[0x%08X] = 0x%04X  %d\n", addr + (i << 1), data, data);
        }
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::r32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = CI_INVALID_NUMBER_OF_ARGUMENTS;

  if (num_args == 2)
  {
    uint32_t addr;
    uint32_t data;
    if(sscanf(cmd_args[1], "%x", &addr) != 1)
    {
      status = CI_INVALID_ARG;
    }
    else if (map(addr & 0xF0000000) != SUCCESS)
    {
      status = HW_CMD_MAP_FAILURE;
    }
    else
    {
      status = hw_reg_p->r32(addr, &data);
      if (status == SUCCESS)
      {
        printf("[0x%08X] = 0x%08X  %d\n", addr, data, data);
      }
    }
  }
  else if (num_args == 3)
  {
    uint32_t addr;
    uint32_t n;
    uint32_t data;
    if((sscanf(cmd_args[1], "%x", &addr) != 1) ||
       (sscanf(cmd_args[2], "%d", &n) != 1))
    {
      status = CI_INVALID_ARG;
    }
    else if (map(addr & 0xF0000000) != SUCCESS)
    {
      status = HW_CMD_MAP_FAILURE;
    }
    else
    {
      for (uint32_t i = 0; i < n; i++)
      {
        status = hw_reg_p->r32(addr + (i << 2), &data);
        if (status == SUCCESS)
        {
          printf("[0x%08X] = 0x%08X  %d\n", addr + (i << 2), data, data);
        }
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::w8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t addr;
  uint8_t data;

  status_t status = FAILURE;

  if((sscanf(cmd_args[1], "%x",   &addr) != 1)  ||
     (sscanf(cmd_args[2], "%hhx", &data) != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (map(addr & 0xF0000000) != SUCCESS)
  {
    status = HW_CMD_MAP_FAILURE;
  }
  else
  {
    status = hw_reg_p->w8(addr, data);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::w16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t addr;
  uint16_t data;

  status_t status = FAILURE;

  if((sscanf(cmd_args[1], "%x",  &addr) != 1)  ||
     (sscanf(cmd_args[2], "%hx", &data) != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (map(addr & 0xF0000000) != SUCCESS)
  {
    status = HW_CMD_MAP_FAILURE;
  }
  else
  {
    status = hw_reg_p->w16(addr, data);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::w32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t addr;
  uint32_t data;

  status_t status = FAILURE;

  if((sscanf(cmd_args[1], "%x", &addr) != 1)  ||
     (sscanf(cmd_args[2], "%x", &data) != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (map(addr & 0xF0000000) != SUCCESS)
  {
    status = HW_CMD_MAP_FAILURE;
  }
  else
  {
    status = hw_reg_p->w32(addr, data);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::s8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t addr;
  uint8_t data;

  status_t status = FAILURE;

  if((sscanf(cmd_args[1], "%x",   &addr) != 1)  ||
     (sscanf(cmd_args[2], "%hhx", &data) != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (map(addr & 0xF0000000) != SUCCESS)
  {
    status = HW_CMD_MAP_FAILURE;
  }
  else
  {
    status = hw_reg_p->s8(addr, data);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::s16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t addr;
  uint16_t data;

  status_t status = FAILURE;

  if((sscanf(cmd_args[1], "%x",  &addr) != 1)  ||
     (sscanf(cmd_args[2], "%hx", &data) != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (map(addr & 0xF0000000) != SUCCESS)
  {
    status = HW_CMD_MAP_FAILURE;
  }
  else
  {
    status = hw_reg_p->s16(addr, data);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::s32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t addr;
  uint32_t data;

  status_t status = FAILURE;

  if((sscanf(cmd_args[1], "%x", &addr) != 1)  ||
     (sscanf(cmd_args[2], "%x", &data) != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (map(addr & 0xF0000000) != SUCCESS)
  {
    status = HW_CMD_MAP_FAILURE;
  }
  else
  {
    status = hw_reg_p->s32(addr, data);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::c8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t addr;
  uint8_t data;

  status_t status = FAILURE;

  if((sscanf(cmd_args[1], "%x",   &addr) != 1)  ||
     (sscanf(cmd_args[2], "%hhx", &data) != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (map(addr & 0xF0000000) != SUCCESS)
  {
    status = HW_CMD_MAP_FAILURE;
  }
  else
  {
    status = hw_reg_p->c8(addr, data);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::c16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t addr;
  uint16_t data;

  status_t status = FAILURE;

  if((sscanf(cmd_args[1], "%x",  &addr) != 1)  ||
     (sscanf(cmd_args[2], "%hx", &data) != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (map(addr & 0xF0000000) != SUCCESS)
  {
    status = HW_CMD_MAP_FAILURE;
  }
  else
  {
    status = hw_reg_p->c16(addr, data);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::c32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  uint32_t addr;
  uint32_t data;

  status_t status = FAILURE;

  if((sscanf(cmd_args[1], "%x", &addr) != 1)  ||
     (sscanf(cmd_args[2], "%x", &data) != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (map(addr & 0xF0000000) != SUCCESS)
  {
    status = HW_CMD_MAP_FAILURE;
  }
  else
  {
    status = hw_reg_p->c32(addr, data);
  }
  return status;
}

//------------------------------------------------------------------------------

extern ci* ci_p;

status_t cmd::source(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = CI_INVALID_NUMBER_OF_ARGUMENTS;

  if (num_args == 2)
  {
    FILE* fptr = fopen(cmd_args[1], "r");
    if (fptr == NULL)
    {
      status = CI_INVALID_ARG;
    }
    else
    {
      // read and process commands from specified file
      bool done = false;
      ci_buf_t cmd_buf;
      do
      {
        if (feof(fptr))
        {
          done = true;
        }
        else
        {
          if (fgets(cmd_buf, CMD_BUF_LENGTH, fptr) == NULL)
          {
            done = true;
          }
          else
          {
            printf("%s: %s", cmd_args[1], cmd_buf);
            status = ci_p->process_cmd(cmd_buf);
            ci_p->process_status(cmd_buf, status);
          }
        }
      } while (!done);
      fclose(fptr);
    }
  }

  return status;
}

//------------------------------------------------------------------------------

status_t cmd::mf8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t addr;
  uint32_t seed;
  uint32_t n;
  if((sscanf(cmd_args[1], "%x",   &addr) != 1) ||
     (sscanf(cmd_args[2], "%ld",  &n)    != 1) ||
     (sscanf(cmd_args[3], "%x",   &seed) != 1) ||
     (seed > 0xFF))
  {
    status = CI_INVALID_ARG;
  }
  else if (n > 0)
  {
    status = map(addr & 0xF0000000);
    if (status == SUCCESS)
    {
      volatile uint8_t* virt_addr = (uint8_t*)mem_map_p->get_virtual_addr(addr);
      uint32_t i;
      for (i = 0; i < n; i++)
      {
        *virt_addr++ = seed + i;
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::mf16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t addr;
  uint32_t seed;
  uint32_t n;
  if((sscanf(cmd_args[1], "%x",  &addr) != 1) ||
     (sscanf(cmd_args[2], "%ld", &n)    != 1) ||
     (sscanf(cmd_args[3], "%x", &seed)  != 1) ||
     (seed > 0xFFFF) ||
     (addr & 0x1))
  {
    status = CI_INVALID_ARG;
  }
  else if (n > 0)
  {
    status = map(addr & 0xF0000000);
    if (status == SUCCESS)
    { 
      volatile uint16_t* virt_addr = (uint16_t*)mem_map_p->get_virtual_addr(addr);
      uint32_t i;
      for (i = 0; i < n; i++)
      {
        *virt_addr++ = seed + i;
      } 
    }
  } 
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::mf32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t addr;
  uint32_t seed;
  uint32_t n;
  if((sscanf(cmd_args[1], "%x", &addr) != 1) ||
     (sscanf(cmd_args[2], "%ld", &n) != 1) ||
     (sscanf(cmd_args[3], "%x", &seed) != 1)||
     (addr & 0x3))
  {
    status = CI_INVALID_ARG;
  }
  else if (n > 0)
  {
    status = map(addr & 0xF0000000);
    if (status == SUCCESS)
    {
      volatile uint32_t* virt_addr = (uint32_t*)mem_map_p->get_virtual_addr(addr);
      uint32_t i;
      for (i = 0; i < n; i++)
      {
        *virt_addr++ = seed + i;
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::ms8(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t addr;
  uint32_t value;
  uint32_t n;
  if((sscanf(cmd_args[1], "%x",   &addr) != 1) ||
     (sscanf(cmd_args[2], "%ld",  &n)    != 1) ||
     (sscanf(cmd_args[3], "%x",   &value) != 1) ||
     (value > 0xFF))
  {
    status = CI_INVALID_ARG;
  }
  else if (n > 0)
  {
    status = map(addr & 0xF0000000);
    if (status == SUCCESS)
    {
      volatile uint8_t* virt_addr = (uint8_t*)mem_map_p->get_virtual_addr(addr);
      uint32_t i;
      for (i = 0; i < n; i++)
      {
        *virt_addr++ = value;
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::ms16(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t addr;
  uint32_t value;
  uint32_t n; 
  if((sscanf(cmd_args[1], "%x",   &addr) != 1) ||
     (sscanf(cmd_args[2], "%ld",  &n)    != 1) ||
     (sscanf(cmd_args[3], "%x",   &value) != 1) ||
     (value > 0xFFFF) ||
     (addr & 0x1))
  {
    status = CI_INVALID_ARG;
  }
  else if (n > 0)
  {
    status = map(addr & 0xF0000000);
    if (status == SUCCESS)
    {
      volatile uint16_t* virt_addr = (uint16_t*)mem_map_p->get_virtual_addr(addr);
      uint32_t i;
      for (i = 0; i < n; i++)
      {
        *virt_addr++ = value;
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::ms32(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t addr;
  uint32_t value;
  uint32_t n; 
  if((sscanf(cmd_args[1], "%x",   &addr)  != 1) ||
     (sscanf(cmd_args[2], "%ld",  &n)     != 1) ||
     (sscanf(cmd_args[3], "%x",   &value) != 1) ||
     (addr & 0x3))
  {
    status = CI_INVALID_ARG;
  }
  else if (n > 0)
  {
    status = map(addr & 0xF0000000);
    if (status == SUCCESS)
    {
      volatile uint32_t* virt_addr = (uint32_t*)mem_map_p->get_virtual_addr(addr);
      uint32_t i;
      for (i = 0; i < n; i++)
      {
        *virt_addr++ = value;
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::mcpy(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t dest_addr;
  uint32_t src_addr;
  uint32_t size;
  if((sscanf(cmd_args[1], "%x",   &dest_addr) != 1) ||
     (sscanf(cmd_args[2], "%x",   &src_addr)  != 1) ||
     (sscanf(cmd_args[3], "%ld",   &size)      != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (size > 0)
  {
    status = map(dest_addr & 0xF0000000);
    if (status == SUCCESS)
    {
      if (!mem_map_p->is_mapped(dest_addr) || (!mem_map_p->is_mapped(src_addr)))
      {
        status = CI_INVALID_ARG;
      }
      else
      {
        void* virt_dest_addr = (void*)mem_map_p->get_virtual_addr(dest_addr);
        void* virt_src_addr = (void*)mem_map_p->get_virtual_addr(src_addr);
        memcpy(virt_dest_addr, virt_src_addr, size);
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::mcmp(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t dest_addr;
  uint32_t src_addr;
  uint32_t size;
  if((sscanf(cmd_args[1], "%x",   &dest_addr) != 1) ||
     (sscanf(cmd_args[2], "%x",   &src_addr)  != 1) ||
     (sscanf(cmd_args[3], "%ld",   &size)      != 1))
  {
    status = CI_INVALID_ARG;
  }
  else if (size > 0)
  {
    status = map(dest_addr & 0xF0000000);
    if (status == SUCCESS)
    {
      if (!mem_map_p->is_mapped(dest_addr) || (!mem_map_p->is_mapped(src_addr)))
      {
        status = CI_INVALID_ARG;
      }
      else
      {
        uint8_t* virt_dest_addr = (uint8_t*)mem_map_p->get_virtual_addr(dest_addr);
        uint8_t* virt_src_addr = (uint8_t*)mem_map_p->get_virtual_addr(src_addr);
        for(uint32_t i = 0; i < size; i++)
        {
          if (virt_dest_addr[i] != virt_src_addr[i])
          {
            printf("[0x%08X] = 0x%02X  !=  [0x%08X] = 0x%02X\n", dest_addr + i, virt_dest_addr[i],
                                                                 src_addr + i, virt_src_addr[i]);
          }
        }
        if(bcmp(virt_dest_addr, virt_src_addr, size) == 0)
        {
          printf("equal\n");
        }
        else
        {
          printf("not equal\n");
        }
      }
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::version(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  printf("%s\n", VERSION); // defined in makefile

  return status;
}

//------------------------------------------------------------------------------

status_t cmd::debug(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t level;
  if (num_args == 1)
  {
    printf("debug level = %d\n", debug_get_debug_level());
  }
  else if (sscanf(cmd_args[1], "%ld", &level) != 1)
  {
    status = CI_INVALID_ARG;
  }
  else
  {
    debug_set_debug_level(level);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::pause(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;
  char* msg = (char*)strstr(cmd_line, " ");
  if (msg == NULL)
  {
    status = CI_INVALID_ARG;
  }
  else
  {
    msg++; // move past the space character
    msg[strlen(msg) - 1] = 0; // replace \n character at end of input
    printf("%s - hit enter to continue\n", msg);
  }
  while (getchar() != '\n');
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::sleep(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;

  uint32_t seconds = 0;
  if (sscanf(cmd_args[1], "%lu", &seconds) != 1)
  {
    status = CI_INVALID_ARG;
  }
  else
  {    
    printf("sleeping for %lu seconds...\n", seconds);
    ::sleep(seconds);
  }
  return status;
}

//------------------------------------------------------------------------------

#include "omap3_beagle_ioctl.h"

status_t cmd::lcd(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = CI_INVALID_NUMBER_OF_ARGUMENTS;

  if (num_args == 2)
  {
    status = SUCCESS;

    // open device file
    FILE*fptr = fopen("/dev/tlc59108-i2c0", "r");
    if (!fptr)
    {
      return FAILURE;
    }

    if (strcasecmp(cmd_args[1], "on") == 0)
    {
      ioctl(fileno(fptr), OMAP3_BEAGLE_LCD_IOCT_ON);
    }
    else if (strcasecmp(cmd_args[1], "off") == 0)
    {
      ioctl(fileno(fptr), OMAP3_BEAGLE_LCD_IOCT_OFF);
    }
    else if (strcasecmp(cmd_args[1], "toggle") == 0)
    {
      ioctl(fileno(fptr), OMAP3_BEAGLE_LCD_IOCT_TOGGLE);
    }
    else if (strcasecmp(cmd_args[1], "flip") == 0)
    {
      ioctl(fileno(fptr), OMAP3_BEAGLE_LCD_IOCT_FLIP);
    }
    else
    {
      status = CI_INVALID_ARG;
    }
    // close device file
    fclose(fptr);
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::i2c_read(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = CI_INVALID_NUMBER_OF_ARGUMENTS;

  uint8_t addr;
  uint8_t data;

  if (num_args == 2)
  {
    if(sscanf(cmd_args[1], "%x", &addr) != 1)
    {
      status = CI_INVALID_ARG;
    }
    else
    {
      // open device file
      FILE*fptr = fopen("/dev/tlc59108-i2c0", "r");
      if (!fptr)
      {
        return FAILURE;
      }

      // read data from device addr
      //data = ioctl(fileno(fptr), OMAP3_BEAGLE_I2C_IOCQ_RD, addr);
      data = ioctl(fileno(fptr), OMAP3_BEAGLE_I2C_IOCQ_RD);
      printf("[0x%02X] = 0x%02X\n", addr, data);

      // close device file
      fclose(fptr);

      status = SUCCESS;
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t cmd::i2c_write(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = CI_INVALID_NUMBER_OF_ARGUMENTS;

  uint8_t addr;
  uint8_t data;

  if (num_args == 3)
  {
    if((sscanf(cmd_args[1], "%hhx", &addr) != 1)  ||  // addr
       (sscanf(cmd_args[2], "%hhx", &data) != 1))     // data
    {
      status = CI_INVALID_ARG;
    }
    else
    {
      // open device file
      FILE*fptr = fopen("/dev/tlc59108-i2c0", "r");
      if (!fptr)
      {
        return FAILURE;
      }

      // write data to device addr
      ioctl(fileno(fptr), OMAP3_BEAGLE_I2C_IOCT_WR, (data << 8) | addr);

      // close device file
      fclose(fptr);

      status = SUCCESS;
    }
  }
  return status;
}

//------------------------------------------------------------------------------

#if 0

status_t cmd::hello(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;
  printf ("%s\n", "World!");
  return status;
}

#endif
