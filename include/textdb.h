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
#include <sstream>
#include <utility>
#include <map>
#include <set>
#include <memory>
#include <regex>

/// This class represents a database / file
class textdb
{
	
	// public static functions 
	public:
		/// Splits a string into a vector at the given delimiter
		static int string_to_vector( std::string& in, std::vector< std::string >& out, char delimiter );

		/// Counts the number of consecutive characters c at the front of string in
		static unsigned int count_char_at_front( std::string& in, char c );

		/** Compare vectors by element
		 * checks only the first v2.size() elements from v1
		 * \returns false if v2.size() > v1.size() or different elements in v2 and v1
		 */
		static bool compare_vectors( std::vector< std::string > v1, std::vector< std::string > v2 );

		/** Compare vectors by element
		 * \returns false if v1.size() != v2.size() or different elements in v2 and v1
		 */
		static bool compare_vectors_exact( std::vector< std::string > v1, std::vector< std::string > v2 );

		/** Compare vectors by element, treats elements of v2 as regex,
		 * checks only the first v2.size() elements from v1
		 * \returns false if v2.size() > v1.size() or v2 doesn't describe v1
		 */
		static bool compare_vectors_regex( std::vector< std::string > v1, std::vector< std::string > v2 );

		/** Compare vectors by element, treats elements of v2 as regex
		 * \returns false if v1.size() != v2.size() or v2 doesn't describe v1
		 */
		static bool compare_vectors_regex_exact( std::vector< std::string > v1, std::vector< std::string > v2 );
	
	public:
		
		/// The keys (as a path) used to identify values
		typedef std::vector< std::string > keys;
		
		/// The values type, holds values associated with a single key
		//typedef std::set< std::string > values;
		typedef std::vector< std::string > values;
		
		/// Returns a reference to the _items map
		std::map< keys, values >& items() { return _items; }
		/// Returns _delimiter
		char delimiter() { return _delimiter; }
		
		/// Print everything
		void print( std::ostream& output, bool color );
		/// Print specified key
		void print( std::ostream& output, bool color, const keys& item_keys );
		
		/// Load a database from a file
		void load( std::istream& input ); // TODO!: merge/replace
		
		/// Export database in graphviz format
		void to_graphviz( std::ostream& output );
		
		/// Export database in tsv format
		void to_tsv( std::ostream& output );
		
	private:
		
		/// All items
		std::map< keys, values > _items;
		
		/// The delimiter used in the file
		char _delimiter = '\t';
		
		std::map< std::string, std::string > _colors =
		{
			{ "key", "\e[31m" },
			//{ "subkey", "\e[31m" },
			{ "subkey", "" },
			{ "value", "\e[36m" },
			{ "subvalue", "\e[36m" },
			{ "reset", "\e[0m" }
			
		};
	
};

#endif
