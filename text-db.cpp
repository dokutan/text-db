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

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <exception>
#include <regex>

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <unistd.h> // for isatty()

#include "include/textdb.h"
#include "include/frontend.h"

// version string fallback, change version in makefile
#ifndef VERSION_STRING
#define VERSION_STRING "undefined"
#endif

void interactive_session( textdb& db, std::map< std::string, std::string >& options );
void pipe_session( textdb& db, std::map< std::string, std::string >& options );

int main( int argc, char* argv[] )
{
	
	// main database object
	textdb db;
	
	// various runtime options
	std::map< std::string, std::string > options =
	{
		{ "ps1", ">> " },
		{ "color", "on" },
		{ "regex", "on" }
	};
	
	// check arguments, load file
	if( argc >= 2 )
	{
		// --help
		if( strcmp( argv[1], "--help" ) == 0 || strcmp( argv[1], "-h" ) == 0 )
		{
			command_help( std::cout );
			return 0;
		}
		
		// load database from stdin
		else if( strcmp( argv[1], "-" ) == 0 )
		{
			db.clear();
			db.load( std::cin );
		}
		
		// load database from specified file
		else
		{
			std::ifstream infile( argv[1] );
			if( !infile.is_open() )
			{
				std::cerr << "Could not open " << argv[1] << "\n";
				return 1;
			}
			
			options["file"] = argv[1];
			db.clear();
			db.load( infile );
			infile.close();
		}
	}
	
	// check arguments, execute command from commandline
	if( argc >= 3 )
	{
		std::string command;
		for( int i = 2; i < argc; i++ )
			command += argv[i];
		
		if( isatty( fileno(stdout) ) )
			process_input( command, db, options, std::cout );
		else if( errno == ENOTTY )
		{
			options["color"] = "off";
			process_input( command, db, options, std::cout );
		}
		
		return 0;
	}
	
	// is stdout connected to a pipe ?
	if( isatty( fileno(stdout) ) )
		interactive_session( db, options );
	else if( errno == ENOTTY )
	{
		options["color"] = "off";
		pipe_session( db, options );
	}
	
	return 0;
}

void interactive_session( textdb& db, std::map< std::string, std::string >& options )
{
	// print intro
	std::cout << "text-db rewrite interactive mode, version: " << VERSION_STRING << "\n";
	std::cout << "Type help for usage information.\n";
	
	// input buffer
	std::string input;
	
	//main loop, process user input
	while(1)
	{
		std::cout << options.at("ps1");
		std::getline( std::cin, input, '\n' );
		
		if( !std::cin.bad() && !std::cin.eof() )
			process_input( input, db, options, std::cout );
		else
			break;
	}
}

void pipe_session( textdb& db, std::map< std::string, std::string >& options )
{
	// input buffer
	std::string input;
	
	//main loop, process user input
	while(1)
	{
		std::getline( std::cin, input, '\n' );
		
		if( !std::cin.bad() && !std::cin.eof() )
			process_input( input, db, options, std::cout );
		else
			break;
	}
}
