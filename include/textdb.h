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

// Main class header

#ifndef TEXTDB
#define TEXTDB

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <memory>

#include "utils.h"

/// This class represents a database / file
class textdb
{
	
	public:
		
		/// The item class
		class dbitem;
		
		/// The path used to identify items
		typedef std::vector< std::string > path;
		
		/// Returns a reference to the _items map
		std::map< path, dbitem >& items() { return _items; }
		/// Returns a reference to _delimiter
		char delimiter() { return _delimiter; }
		
		/// Store a top-level item in the database
		int store( textdb::dbitem item ); // TODO!: merge/replace
		/// Store an item in the database
		int store( textdb::dbitem item, const path& parent ); // TODO!: merge/replace
		
		/// Print everything
		void print( std::ostream& output, bool color );
		void print_item( std::ostream& output, bool color, const path& itempath );
		
		/// Load a database from a file
		void load( std::istream& input ); // TODO!: merge/replace
		
		/// Delete the contents of the database
		void clear();
		
	private:
		
		/// The delimiter used in the file
		char _delimiter = '\t';
		
		/// All items
		std::map< path, dbitem > _items;
		
		std::map< std::string, std::string > _colors =
		{
			{ "key", "\e[31m" },
			{ "subkey", "\e[31m" },
			{ "value", "\e[36m" },
			{ "subvalue", "\e[36m" },
			{ "reset", "\e[0m" }
			
		};
	
};

/// This class represents an item
class textdb::dbitem
{
	
	public:
		
		/// Constructor
		dbitem( std::string key, std::vector< std::string > values );
		/// Empty constructor
		dbitem();
		
		/// Return a reference to the key of this item
		std::string& key() { return _key; };
		/// Return a reference to the values of this item
		std::vector< std::string >& values() { return _values; };
			
	private:
		
		std::string _key;
		std::vector< std::string > _values;
	
};

#endif
