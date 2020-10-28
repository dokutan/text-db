/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

// Frontend functions header

#ifndef TEXTDB_FRONTEND
#define TEXTDB_FRONTEND 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <regex>
#include <set>
#include <exception>

#include "textdb.h"

/** Takes a line of user input and performs the specified actions on the database
 * Simple actions (e.g. print) are performed directly from this function.
 * For more complicated actions (e.g. search) the command_* functions are called.
 */
void process_input( std::string& input, textdb& db, std::map< std::string, std::string >& options, std::ostream& output );


// command functions, these are used to perform more complicated actions

/// Prints the available commands
void command_help( std::ostream& output );

/// Prints the number of items in the database
void command_count( std::ostream& output, textdb& db );

/** Search and print items with matching keys
 * \arg keys the delimiter separated fields of the search term
 */
void command_search_by_key( std::string& keys, textdb& db, std::ostream& output, bool use_color, bool use_regex );

/** Search and print items with matching keys and values
 * \arg keys the delimiter separated fields of the key search term
 * \arg values the delimiter separated fields of the value search term
 */ 
void command_search_by_key_value( std::string& keys, std::string& values, textdb& db, std::ostream& output, bool use_color, bool use_regex );

/// Load database from a file
void command_load_file( std::string& filename, textdb& db, std::map< std::string, std::string >& options, std::ostream& output );

/// Save database to a file
void command_save_file( std::string& filename, textdb& db, std::ostream& output );

/// Add values to the specified keys
void command_add_values( std::string& keys, std::string& values, textdb& db, std::ostream& output, bool use_regex );

/// Delete the specified keys
void command_delete_keys( std::string& keys, textdb& db, std::ostream& output, bool use_regex );

/// Delete the specified values from the specified keys
void command_delete_values( std::string& keys, std::string& values, textdb& db, std::ostream& output, bool use_regex );

/// Move or rename the specified keys
void command_rename_keys( std::string& keys_old, std::string& keys_new, textdb& db, std::ostream& output, bool use_regex );

/// Copy the specified keys
void command_copy_keys( std::string& keys_old, std::string& keys_new, textdb& db, std::ostream& output, bool use_regex );

/// Show the values of the specified keys
void command_show_values( std::string& keys, textdb& db, std::ostream& output, bool use_regex );

/// Add keys as subkeys of existing keys
void command_add_keys( std::string& key_string, std::string& key_new_string, textdb& db, std::ostream& output, bool use_regex );

#endif
