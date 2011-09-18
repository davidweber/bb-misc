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

#ifndef __CI_H__
#define __CI_H__

#include <stdint.h>
#include "cmd_table.h"
#include "status.h"
#include "libtecla.h"

typedef char  ci_buf_t[CMD_BUF_LENGTH];

/**
 * @class ci
 *
 * @brief Command interpreter class definition
 */ 
class ci
{
  private:
    cmd_table_entry_t* cmd_table;
    uint32_t num_cmds;
    const char* prompt_string;
    GetLine* get_line;

  protected:

  public:

  ci(cmd_table_entry_t* cmd_table, uint32_t num_cmds, const char* prompt_string);

  ~ci(); 

  /**
   * @brief Initializes the command interpreter 
   *
   * @return void
   */
  void init();

  /**
   * @brief The main loop of the command interpreter 
   *
   * @return void
   */
  void run();


  /**
   * @brief Invokes function associated with a command 
   *
   * @param line The command line string 
   * @param cmd_args The command line string separated into arguments
   * @param num_args The number of arguments in cmd_args array
   *
   * @return SUCCESS or error code
   */
  status_t handler(const char* line, arg_array_t cmd_args, uint32_t num_args);


  /**
   * @brief Acquires a command from the command input source 
   *
   * @param buf buffer containing acquired command
   *
   * @return SUCCESS or error code
   */
  status_t receive_cmd(ci_buf_t buf);


  /**
   * @brief Parses the cmd_line parameter and invokes associated function handler 
   *
   * @param  
   *
   * @return SUCCESS or error code
   */
  status_t process_cmd(const char* cmd_line);


  /**
   * @brief Sends response to a command 
   *
   * @return void
   */
  void send_response();


  /**
   * @brief Handles error returned from process commands 
   *
   * @param status SUCCESS or error code
   *
   * @return 
   */
  void process_status(const char* cmd_line, status_t status);


  /**
   * @brief Prints the command's arguments 
   *
   * @param args array of argument strings 
   * @param num_args number of arguments present in args array
   *
   * @return void
   */
  void print_args(arg_array_t args, uint32_t num_args);


  /**
   * @brief Prints command summary 
   *
   * @return void
   */
  void help(void);
};

#endif //__CI_H__

