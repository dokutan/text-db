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
		for( auto value : item.second )
			output << _delimiter << color_value << value << color_reset;
		
		output << std::endl;
	}
	
}

void textdb::print( std::ostream& output, bool color, const keys& item_keys )
{
	// iterate over all items
	for( auto item : _items )
	{
		if( !compare_vectors( item.first, item_keys ) )
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
		for( auto value : item.second )
			output << _delimiter << color_value << value << color_reset;
		
		output << std::endl;
	}
	
}

void textdb::load( std::istream& input )
{
	
	keys temp_keys({""});
	
	// iterate over file
	for( std::string line; std::getline( input, line, '\n' ); )
	{
		
		// variables
		unsigned int depth = 0;
		std::vector< std::string > line_parts;
		std::string item_key_last;
		values item_values;
		
		// empty line? → skip
		if( line.size() == 0 )
			continue;
		
		// get depth
		depth = count_char_at_front( line, _delimiter );
		
		// remove leading delimiters from line
		while( 1 )
		{
			if( line.front() == _delimiter )
				line.erase( line.begin() );
			else
				break;
		}
		
		// split line: last element of the key + values
		string_to_vector( line, line_parts, _delimiter );
		
		// sanity check, there has to be at least one element in the vector (key)
		if( line_parts.size() == 0 )
			continue;
		
		// last element of the key
		item_key_last = line_parts.at(0);
		// values
		item_values = values( line_parts.begin()+1, line_parts.end() );
		
		// new item has same parent as previous item
		if( depth+1 == temp_keys.size() )
		{
			if( temp_keys.size() == 0 )
				temp_keys.push_back( item_key_last );
			else
				temp_keys.back() = item_key_last;
			
			//item_keys = temp_keys;
			_items.emplace( temp_keys, item_values );
		}
		
		// new item is child of previous item
		else if( depth == temp_keys.size() )
		{
			temp_keys.push_back( item_key_last );
			_items.emplace( temp_keys, item_values );
		}
		
		// new item is somewhere above previous item
		else if( depth+1 < temp_keys.size() )
		{
			temp_keys.erase( temp_keys.begin()+depth, temp_keys.end() );
			temp_keys.push_back( item_key_last );
			_items.emplace( temp_keys, item_values );
		}
		
	}
		
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

void textdb::to_tsv( std::ostream& output )
{
	for( auto& i : _items )
	{
		for( auto& k : i.first )
			output << k << "\t";
		for( auto& v : i.second )
			output << "\t" << v;
		output << std::endl;
	}
}
