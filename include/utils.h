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

// Utility functions header

#ifndef TEXTDB_UTILS
#define TEXTDB_UTILS 

#include <vector>
#include <string>
#include <sstream>
#include <regex>

/// Splits a string into a vector
int string_to_vector( std::string& in, std::vector< std::string >& out, char delimiter );

/// Counts the number of consecutive c at the front of in
unsigned int count_char_at_front( std::string& in, char c );

bool compare_vectors( std::vector< std::string > v1, std::vector< std::string > v2 );

bool compare_vectors_regex( std::vector< std::string > v1, std::vector< std::string > v2 );

bool compare_vectors_regex_exact( std::vector< std::string > v1, std::vector< std::string > v2 );

#endif
