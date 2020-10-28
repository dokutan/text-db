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

// Utility functions

//#include "utils.h"
#include "textdb.h"

int textdb::string_to_vector( std::string& in, std::vector< std::string >& out, char delimiter )
{
	std::stringstream instream( in );
	out.clear();
	
	for( std::string line; std::getline( instream, line, delimiter ); )
		out.push_back( line );
	
	return out.size();
}

unsigned int textdb::count_char_at_front( std::string& in, char c )
{
	unsigned int count = 0;
	for( auto i : in )
	{
		if( i == c )
			count++;
		else
			break;
	}
	
	return count;
}

bool textdb::compare_vectors( std::vector< std::string > v1, std::vector< std::string > v2 )
{
	if( v2.size() > v1.size() )
		return false;
	
	for( size_t i = 0; i < v2.size(); i++ )
	{
		if( v1.at(i) != v2.at(i) )
			return false;
	}
	
	return true;
}

bool textdb::compare_vectors_exact( std::vector< std::string > v1, std::vector< std::string > v2 )
{
	if( v2.size() != v1.size() )
		return false;
	
	for( size_t i = 0; i < v2.size(); i++ )
	{
		if( v1.at(i) != v2.at(i) )
			return false;
	}
	
	return true;
}

bool textdb::compare_vectors_regex( std::vector< std::string > v1, std::vector< std::string > v2 )
{
	if( v2.size() > v1.size() )
		return false;
	
	for( size_t i = 0; i < v2.size(); i++ )
	{
		if( !std::regex_match( v1.at(i), std::regex( v2.at(i) ) ) )
			return false;
	}
	
	return true;
}

bool textdb::compare_vectors_regex_exact( std::vector< std::string > v1, std::vector< std::string > v2 )
{
	if( v2.size() != v1.size() )
		return false;
	
	for( size_t i = 0; i < v2.size(); i++ )
	{
		if( !std::regex_match( v1.at(i), std::regex( v2.at(i) ) ) )
			return false;
	}
	
	return true;
}
