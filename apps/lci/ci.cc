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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/poll.h>
#include "ci.hh" 
#include "debug.h"
#include "libtecla.h"
#include "cmd_table.h"

#define HISTORY_FILE "~/.lci.history"

//------------------------------------------------------------------------------

ci::ci(cmd_table_entry_t* cmd_table, uint32_t num_cmds, const char* prompt_string)
{
  this->cmd_table     = cmd_table;
  this->num_cmds      = num_cmds;
  this->prompt_string = prompt_string;
  this->get_line = new_GetLine(1024, 2048);
  gl_load_history(this->get_line, HISTORY_FILE, "history file");

  struct pollfd fds;
  fds.fd = 0; /* this is STDIN */
  fds.events = POLLIN;
  this->pipe_input = (poll(&fds, 1, 0) == 1);
}

//------------------------------------------------------------------------------

ci::~ci()
{
  if (this->get_line)
  {
    gl_save_history(this->get_line, HISTORY_FILE, "history file", 100);
    del_GetLine(this->get_line);
  }
}

//------------------------------------------------------------------------------

void ci::init()
{
}

//------------------------------------------------------------------------------

void ci::run()
{
  ci_buf_t ci_buf;
  bool done = false;
  status_t status = SUCCESS;

  while(!done)
  {
    if (feof(stdin))
    {
      done = true;
    }
    else
    {
      status = receive_cmd(ci_buf);
      if(status == SUCCESS)
      {
        status = process_cmd(ci_buf);
        process_status(ci_buf, status);
        if(status == EXIT)
        {
          done = true;
        }
      }
      else
      {
        done = true;
      }
    }
  }
}

//------------------------------------------------------------------------------

status_t ci::handler(const char* cmd_line, arg_array_t cmd_args, uint32_t num_args)
{
  status_t status = SUCCESS;
  uint8_t i;

  for(i = 0; i < num_cmds; i++)
  {
    if(strcasecmp(cmd_args[0], cmd_table[i].cmd) != 0)
    {
      status = CI_UNRECOGNIZED_COMMAND;
    }
    else if (((num_args < cmd_table[i].min_args) || 
             (num_args > cmd_table[i].max_args)) && 
             ((cmd_table[i].min_args != 0) && (cmd_table[i].max_args != 0)))
    {
      status = CI_INVALID_NUMBER_OF_ARGUMENTS;
      break;
    }
    else
    {
      status = cmd_table[i].cmd_handler(cmd_line, cmd_args, num_args);
      break;
    }
  }
  if (status == CI_UNRECOGNIZED_COMMAND)
  {
    if (system(cmd_line) != -1)
    {
      status = SUCCESS;
    }
  }
  return status;
}

//------------------------------------------------------------------------------

char prompt_g[1024];

status_t ci::receive_cmd(ci_buf_t buf)
{
  status_t status = SUCCESS;
  char* line = NULL;
  if (this->get_line == NULL)
  {
    status = FAILURE;
  }
  else
  {
    char* pwd = get_current_dir_name();
    char* user = getenv("LOGNAME");
    sprintf(prompt_g, "[%s@%s] %s # ", user, this->prompt_string, pwd);
    line = gl_get_line(this->get_line, prompt_g, NULL, -1);
    if (line == NULL)
    {
      status = FAILURE;
    }
    else
    {
      if (this->pipe_input)
      {
        printf("%s", line);
      }
      strcpy(buf, line);
    }
  }
  return status;
}

//------------------------------------------------------------------------------

status_t ci::process_cmd(const char* cmd_line)
{
  status_t status = SUCCESS;
  const char* scanf_format_string = "%s %s %s %s %s %s %s %s";
  arg_array_t cmd_args = {0,0,0,0,0,0,0,0};
  uint16_t num_args = 0;

  if ((strcmp("\n", cmd_line) != 0) && (cmd_line[0] != '#'))
  {
    num_args = sscanf(cmd_line, scanf_format_string,
                      cmd_args[0], cmd_args[1],
                      cmd_args[2], cmd_args[3],
                      cmd_args[4], cmd_args[5],
                      cmd_args[6], cmd_args[7]);

    if(num_args > 0)
    {
      print_args(cmd_args, num_args);
      status = handler(cmd_line, cmd_args, num_args);
    }
  }
  return status;
}

//------------------------------------------------------------------------------

void ci::send_response()
{
}

//------------------------------------------------------------------------------

void ci::process_status(const char* cmd_line, status_t status)
{
  switch(status)
  {
    case CI_INVALID_NUMBER_OF_ARGUMENTS:
      fprintf(stderr, "Error - invalid number of arguments\n");
      fprintf(stderr, "%s\n", cmd_line);
      break;

    case CI_INVALID_ARG:
      fprintf(stderr, "Error - invalid argument\n");
      fprintf(stderr, "%s\n", cmd_line);
      break;

    case CI_UNRECOGNIZED_COMMAND:
      fprintf(stderr, "Error - unrecognized command\n");
      fprintf(stderr, "%s\n", cmd_line);
      break;

    case CI_INVALID_HW_REG_OBJECT:
      fprintf(stderr, "Error - invalid hw_reg object, run \"map\" command\n");
      fprintf(stderr, "%s\n", cmd_line);
      break;

    case HW_INVALID_ADDRESS:
      fprintf(stderr, "Error - invalid address\n");
      fprintf(stderr, "%s\n", cmd_line);
      break;

    case HW_INVALID_MEM_MAP_OBJECT:
      fprintf(stderr, "Error - invalid mem_map object, run \"map\" command\n");
      fprintf(stderr, "%s\n", cmd_line);
      break;

    case HW_CMD_MAP_FAILURE:
      fprintf(stderr, "Error - unable to map specified address\n");
      fprintf(stderr, "%s\n", cmd_line);
      break;

    case SUCCESS:
      break;

    case EXIT:
      fprintf(stderr, "\n");
      break;

    case FAILURE:
      fprintf(stderr, "Error - operation failed\n");
      fprintf(stderr, "%s\n", cmd_line);
      break;

    default:
      fprintf(stderr, "Error - unrecognized error - %d\n", status);
      fprintf(stderr, "%s\n", cmd_line);
      break;
  }
}

//------------------------------------------------------------------------------

void ci::print_args(arg_array_t args, uint32_t num_args)
{
  uint8_t i = 0;

  DPRINTF(DBG_LVL_CI, "num_args: %d\n", num_args);
  for(i = 0; i < num_args; i++)
  {
    DPRINTF(DBG_LVL_CI, "arg[%d] = \"%s\"\n", i, args[i]);
  }
}

//------------------------------------------------------------------------------

void ci::help()
{
  int i;
  printf("\n");
  printf("  Command Summary\n");
  printf("------------------------------------------------------------------------\n");
  printf("\n");
  for(i = 0; i < num_cmds; i++)
  {
    printf("  %s\n", cmd_table[i].desc);
  }
  printf("\n");
  printf("------------------------------------------------------------------------\n");
}

