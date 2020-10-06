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

// Member functions for textdb

#include "textdb.h"

int textdb::store( textdb::dbitem item )
{
	_items.emplace( path({item.key()}), item );
	return 0;
}

int textdb::store( textdb::dbitem item, const textdb::path& parent )
{
	path newpath = parent;
	newpath.push_back( item.key() );
	
	_items.emplace( newpath, item );
	
	return 0;
}

void textdb::print( std::ostream& output, bool color )
{
	// print all items
	for( auto item : _items )
	{
		// determine correct escape codes for color
		std::string color_key = color ? item.first.size() == 1 ? _colors.at("key") : _colors.at("subkey") : "";
		std::string color_value = color ? item.first.size() == 1 ? _colors.at("value") : _colors.at("subvalue") : "";
		std::string color_reset = color ? _colors.at("reset") : "";
		
		// padding
		for( auto i = item.first.size(); i > 1; i-- )
			output << _delimiter;
		
		// key
		output << color_key << item.first.back() << color_reset;
		
		// values
		for( auto value : item.second.values() )
			output << _delimiter << color_value << value << color_reset;
		
		output << std::endl;
	}
	
}

void textdb::print_item( std::ostream& output, bool color, const path& itempath )
{
	// iterate over all items
	for( auto item : _items )
	{
		if( !compare_vectors( item.first, itempath ) )
			continue;
		
		// determine correct escape codes for color
		std::string color_key = color ? item.first.size() == 1 ? _colors.at("key") : _colors.at("subkey") : "";
		std::string color_value = color ? item.first.size() == 1 ? _colors.at("value") : _colors.at("subvalue") : "";
		std::string color_reset = color ? _colors.at("reset") : "";
		
		// padding
		for( auto i = item.first.size(); i > 1; i-- )
			output << _delimiter;
		
		// key
		output << color_key << item.first.back() << color_reset;
		
		// values
		for( auto value : item.second.values() )
			output << _delimiter << color_value << value << color_reset;
		
		output << std::endl;
	}
	
}

void textdb::load( std::istream& input )
{
	
	path buffer_path({""});
	
	// iterate over file
	for( std::string line; std::getline( input, line, '\n' ); )
	{
		// empty line? → skip
		if( line.size() == 0 )
			continue;
		
		// get depth
		unsigned int depth = count_char_at_front( line, _delimiter );
		
		// remove leading delimiters from line
		while( 1 )
		{
			if( line.front() == _delimiter )
				line.erase( line.begin() );
			else
				break;
		}
		
		// split line → key + values
		std::vector< std::string > parts;
		string_to_vector( line, parts, _delimiter );
		
		// sanity check
		if( parts.size() == 0 )
			continue;
		
		std::string item_key = parts.at(0); // key
		parts.erase( parts.begin() ); // values
		
		// path for current item
		path item_path;
		
		// new item has same parent as previous item
		if( depth+1 == buffer_path.size() )
		{
			if( buffer_path.size() == 0 )
				buffer_path.push_back( item_key );
			else
				buffer_path.back() = item_key;
			
			item_path = buffer_path;
			_items.emplace( buffer_path, dbitem(item_key, parts) );
		}
		
		// new item is child of previous item
		else if( depth+1 == buffer_path.size()+1 )
		{
			buffer_path.push_back( item_key );
			item_path = buffer_path;
			_items.emplace( buffer_path, dbitem(item_key, parts) );
		}
		
		// new item is somewhere above previous item
		else if( depth+1 < buffer_path.size() )
		{
			for( unsigned int i = 0; i < depth; i++ )
				item_path.push_back( buffer_path.at(i) );
			
			item_path.push_back( item_key );
			buffer_path = item_path;
			
			_items.emplace( buffer_path, dbitem(item_key, parts) );
		}
		
	}
	
}

void textdb::clear()
{
	_items.clear();
}

void textdb::to_graphviz( std::ostream& output )
{
	
	output << "digraph {\n";
	
	// graph state
	bool in_subgraph = false;
	std::string item_name = "";
	// size_t item_number = 0;
	
	// iterate over all items
	for( auto item : _items )
	{
		
		// if top level item
		if( item.first.size() == 1 )
		{
			
			if( in_subgraph )
				output << "\t}\n";
			
			// TODO! " in item name is not handled
			item_name = item.first.back();
			output << "\tsubgraph \"" << item_name << "\" {\n";
			output << "\t\t\"" << item_name << "\";\n";
			in_subgraph = true;
			
		}
		else
		{
			output << "\t\t";
			bool first = true;
			
			// iterate over item path
			for( auto element : item.first )
			{
				if( !first )
					output << " -> ";
				else
					first = false;
				
				output << "\"" << element << "\"";
			}
			
			output << ";\n";
		}
		
	}
	
	
	if( in_subgraph )
		output << "\t}\n";
		
	output << "}\n";
	
	
}
