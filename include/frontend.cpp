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

// Frontend functions

#include "frontend.h"

void process_input( std::string& input, textdb& db, std::map< std::string, std::string >& options, std::ostream& output )
{
	
	// help message
	if( std::regex_match( input, std::regex("help.*") ) )
		print_help( output );
	
	// quit
	else if( std::regex_match( input, std::regex("(quit|close|exit).*") ) )
		exit(0);
	
	// print everything
	else if( std::regex_match( input, std::regex("print.*") ) )
	{
		options["color"] == "on" ? db.print( output, true ) : db.print( output, false );
		return;
	}
	
	// clear database
	else if( std::regex_match( input, std::regex("clear.*") ) )
	{
		db.clear();
		output << "Deleted everything\n";
		return;
	}
	
	// count items
	else if( std::regex_match( input, std::regex("(count|size).*") ) )
	{
		// count top-level items
		size_t top_items = 0;
		for( auto& item : db.items() )
		{
			if( item.first.size() == 1 )
				top_items++;
		}
		
		output << "Number of items (excluding subitems): " << top_items << "\n";
		output << "Number of items (including subitems): " << db.items().size() << "\n";
		return;
	}
	
	// set options
	else if( std::regex_match( input, std::regex("set.*") ) )
	{
		// list all options
		if( std::regex_match( input, std::regex("set[[:s:]]+list.*") ) )
		{
			for( auto o : options )
				output << o.first << "\t" << o.second << "\n";
			
		}
		
		// set option
		else if( std::regex_match( input, std::regex("set[[:s:]][^[:s:]]+[[:s:]].+") ) )
		{
			options[
				std::regex_replace(
					std::regex_replace( input, std::regex("set[[:s:]]+"), "" )
				, std::regex("[[:s:]].*"), "" )
			] = std::regex_replace( input, std::regex("set[[:s:]][^[:s:]]+[[:s:]]"), "" );
		}
		
		// print error
		else
			output << "Invalid usage, type help for more info\n";
		
		return;
	}
	
	// open file
	else if( std::regex_match( input, std::regex("open.*") ) )
	{
		// open file
		if( std::regex_match( input, std::regex("open[[:s:]].+") ) )
		{
			std::string filename = std::regex_replace( input, std::regex("open[[:s:]]"), "" );
			std::ifstream infile( filename );
			
			if( !infile.is_open() )
			{
				output << "Could not open " << filename << "\n";
				return;
			}
			
			options["file"] = filename;
			db.clear();
			db.load( infile );
			infile.close();
			return;
		}
		
		// print currently opened file
		else if( input == "open" )
		{
			if( options.find("file") != options.end() )
				output << options.at("file") << "\n";
		}
		
		// print error
		else
			output << "Invalid usage, type help for more info\n";
		
		return;
	}
	
	// save file
	else if( std::regex_match( input, std::regex("save.*") ) )
	{
		// save to specified file
		if( std::regex_match( input, std::regex("save[[:s:]].+") ) )
		{
			std::string filename = std::regex_replace( input, std::regex("save[[:s:]]"), "" );
			std::ofstream outfile( filename );
			
			if( !outfile.is_open() )
			{
				output << "Could not open " << filename << "\n";
				return;
			}
			
			db.print( outfile, false );
			outfile.close();
			return;
		}
		
		// save to currently opened file
		else if( input == "save" )
		{
			if( options.find("file") != options.end() )
			{
				std::ofstream outfile( options.at("file") );
				
				if( !outfile.is_open() )
				{
					output << "Could not open " << options.at("file") << "\n";
					return;
				}
				
				db.print( outfile, false );
				outfile.close();
				return;
			}
			else
			{
				output << "No file specified\n";
				return;
			}
		}
	}
	
	// search
	else if( std::regex_match( input, std::regex("search.*") ) )
	{
		
		try
		{
			
			// search by key and value
			if( std::regex_match( input, std::regex("search[[:s:]].+\t\t.+") ) )
			{
				// search terms
				std::string term_string = std::regex_replace( input, std::regex("search[[:s:]]"), "" );
				std::string key_string = std::regex_replace( term_string, std::regex("\t\t.*"), "" ); // keys
				std::string value_string = std::regex_replace( term_string, std::regex(".*\t\t"), "" ); // values
				std::vector< std::string > key_terms({}), value_terms({});
				string_to_vector( key_string, key_terms, db.delimiter() );
				string_to_vector( value_string, value_terms, db.delimiter() );
				
				std::set< std::string > results;
				
				// perform search
				for( auto& item : db.items() )
				{
					
					// search by key
					if( compare_vectors_regex_exact( item.first, key_terms ) )
					{
						
						// check values
						bool values_match = false;
						
						// iterate over value search terms
						for( auto value_term : value_terms )
						{
							// iterate over item values
							for( auto value : item.second.values() )
							{
								// check value
								if( std::regex_match( value, std::regex(value_term) ) )
								{
									values_match = true;
									break;
								}
							}
							
							if( !values_match )
								break;
							
						}
						
						// store result
						if( values_match )
							results.emplace( item.first.front() );
					}
				}
				
				// print results
				for( auto& r : results )
					options["color"] == "on" ? db.print_item( output, true, textdb::path({r}) ) : db.print_item( output, false, textdb::path({r}) );
				
				return;
			}
			
			// search by key
			else if( std::regex_match( input, std::regex("search[[:s:]].+") ) )
			{
				// search terms
				std::string term_string = std::regex_replace( input, std::regex("search[[:s:]]"), "" );
				std::vector< std::string > terms({});
				string_to_vector( term_string, terms, db.delimiter() );
				
				std::set< std::string > results;
				
				// perform search
				for( auto& item : db.items() )
				{
					if( compare_vectors_regex_exact( item.first, terms ) )
						results.emplace( item.first.front() );
				}
				
				// print results
				for( auto& r : results )
					options["color"] == "on" ? db.print_item( output, true, textdb::path({r}) ) : db.print_item( output, false, textdb::path({r}) );
				
				return;
			}
			
			else
			{
				output << "No search pattern specified\n";
				return;
			}
			
		}
		catch( std::exception& e )
		{
			output << e.what() << "\n";
			return;
		}
		
	}
	
	// add values
	else if( std::regex_match( input, std::regex("add-values.*") ) )
	{
		try
		{
			
			if( std::regex_match( input, std::regex("add-values[[:s:]].+\t\t.+") ) )
			{
				// search terms
				std::string term_string = std::regex_replace( input, std::regex("add-values[[:s:]]"), "" );
				std::string key_string = std::regex_replace( term_string, std::regex("\t\t.*"), "" );
				std::string value_string = std::regex_replace( term_string, std::regex(".*\t\t"), "" );
				std::vector< std::string > key_terms({}), value_terms({});
				string_to_vector( key_string, key_terms, db.delimiter() );
				string_to_vector( value_string, value_terms, db.delimiter() );
				
				// perform search
				for( auto& item : db.items() )
				{
					if( compare_vectors_regex_exact( item.first, key_terms ) )
					{
						// store values: iterate over value terms
						for( auto value_term : value_terms )
						{
							if( std::find( item.second.values().begin(), item.second.values().end(), value_term ) == item.second.values().end() )
								item.second.values().push_back( value_term );
						}
						
					}
				}
				
				return;
			}
			
			else
			{
				output << "Nothing to add\n";
				return;
			}
			
		}
		catch( std::exception& e )
		{
			output << e.what() << "\n";
			return;
		}
	}
	
	// add items
	else if( std::regex_match( input, std::regex("add-items.*") ) )
	{
		try
		{
			// add subitems
			if( std::regex_match( input, std::regex("add-items[[:s:]].+\t\t.+") ) )
			{
				// search terms
				std::string term_string = std::regex_replace( input, std::regex("add-items[[:s:]]"), "" );
				std::string key_string = std::regex_replace( term_string, std::regex("\t\t.*"), "" );
				std::string key_new_string = std::regex_replace( term_string, std::regex(".*\t\t"), "" );
				std::vector< std::string > key_terms({}), new_keys({});
				string_to_vector( key_string, key_terms, db.delimiter() );
				string_to_vector( key_new_string, new_keys, db.delimiter() );
				
				// perform search
				for( auto& item : db.items() )
				{
					if( compare_vectors_regex_exact( item.first, key_terms ) )
					{
						// store new keys
						for( auto new_key : new_keys )
						{
							textdb::path new_path = item.first;
							new_path.push_back( new_key );
							
							if( db.items().find( new_path ) == db.items().end() )
								db.store( textdb::dbitem( new_key, {} ), item.first );
						}
						
					}
				}
				
				return;
			}
			
			// add top-level items
			else if( std::regex_match( input, std::regex("add-items[[:s:]].+") ) )
			{
				// search terms
				std::string term_string = std::regex_replace( input, std::regex("add-items[[:s:]]"), "" );
				std::vector< std::string > new_keys({});
				string_to_vector( term_string, new_keys, db.delimiter() );
				
				for( auto& new_key : new_keys )
				{
					db.store( textdb::dbitem( new_key, {} ) );
				}
				
				return;
			}
			
			else
			{
				output << "Nothing to add\n";
				return;
			}
			
		}
		catch( std::exception& e )
		{
			output << e.what() << "\n";
			return;
		}
	}
	
	// delete values
	else if( std::regex_match( input, std::regex("rm-values.*") ) )
	{
		
		try
		{
			
			if( std::regex_match( input, std::regex("rm-values[[:s:]].+\t\t.+") ) )
			{
				// search terms
				std::string term_string = std::regex_replace( input, std::regex("rm-values[[:s:]]"), "" );
				std::string key_string = std::regex_replace( term_string, std::regex("\t\t.*"), "" );
				std::string value_string = std::regex_replace( term_string, std::regex(".*\t\t"), "" );
				std::vector< std::string > key_terms({}), value_terms({});
				string_to_vector( key_string, key_terms, db.delimiter() );
				string_to_vector( value_string, value_terms, db.delimiter() );
				
				std::vector< std::string > results;
				
				// perform search, iterate over items
				for( auto& item : db.items() )
				{
					results.clear();
					
					// if paths match
					if( compare_vectors_regex_exact( item.first, key_terms ) )
					{
						// delete values: iterate over item values
						for( auto& value : item.second.values() )
						{
							//if( std::find( item.second.values().begin(), item.second.values().end(), value_term ) != item.second.values().end() )
							//	item.second.values().erase( std::find( item.second.values().begin(), item.second.values().end(), value_term ) );
							
							// iterate over item value terms
							for( auto& value_term : value_terms )
							{
								// check value
								if( std::regex_match( value, std::regex(value_term) ) )
								{
									results.push_back( value );
									//item.second.values().erase( std::find( item.second.values().begin(), item.second.values().end(), value ) );
								}
							}
							
						}
						
					}
					
					// delete results
					for( auto& r : results )
						item.second.values().erase( std::find( item.second.values().begin(), item.second.values().end(), r ) );
					
				}
				
				
				return;
			}
			
			else
			{
				output << "Nothing to delete\n";
				return;
			}
			
		}
		catch( std::exception& e )
		{
			output << e.what() << "\n";
			return;
		}
		
	}
	
	// delete items
	else if( std::regex_match( input, std::regex("rm-items.*") ) )
	{
		
		try
		{
			if( std::regex_match( input, std::regex("rm-items[[:s:]].+") ) )
			{
				// search terms
				std::string term_string = std::regex_replace( input, std::regex("rm-items[[:s:]]"), "" );
				std::vector< std::string > delete_keys({});
				string_to_vector( term_string, delete_keys, db.delimiter() );
				
				std::vector< textdb::path > results;
				
				// iterate over items
				for( auto& item : db.items() )
				{
					if( compare_vectors_regex( item.first, delete_keys ) )
						results.push_back( item.first );
				}
				
				// delete results
				for( auto& r : results )
					db.items().erase( r );
				
				return;
			}
			
			else
			{
				output << "Nothing to delete\n";
				return;
			}
			
		}
		catch( std::exception& e )
		{
			output << e.what() << "\n";
			return;
		}
		
	}
	
	// export to graphviz
	else if( std::regex_match( input, std::regex("export[[:s:]]+gv") ) )
	{
		db.to_graphviz( std::cout );
	}
	
	else
		output << "Unknown command, type help for more info\n";
	
}

void print_help( std::ostream& output )
{
	output <<
	"Usage:\n"
	"text-db [FILE] [COMMAND]\tload file, optionally run command\n" <<
	"text-db - [COMMAND]\t\tload database from stdin, optionally run command\n" <<
	"text-db --help\t\t\tshow this message\n\n";
	
	output <<
	"Available commands:\n" <<
	"open [FILE]\t\topen a file or show the currently opened file\n" <<
	"save [FILE]\t\tsave to specified or currently opened file\n" <<
	"print\t\t\tshow the complete database\n" <<
	"clear\t\t\tdelete everything in the database\n" <<
	"count\t\t\tshow the number of items in the database\n" <<
	"set list\t\tlist all options\n" <<
	"set OPTION VALUE\tset an option to a value\n" <<
	"search KEYS [VALUES]\tsearch for items, 1 tab to separate fields\n" <<
	"\t\t\t2 tabs to separate keys and values\n" <<
	"add-values KEYS VALUES\tadd values, 1 tab to separate fields\n" <<
	"\t\t\t2 tabs to separate keys and values\n" <<
	"add-items KEYS NEW_KEYS\tadd items, 1 tab to separate fields\n" <<
	"\t\t\t2 tabs to separate keys and new keys\n" <<
	"rm-values KEYS VALUES\tdelete values, 1 tab to separate fields\n" <<
	"\t\t\t2 tabs to separate keys and new keys\n" <<
	"rm-items KEYS\t\tdelete items, 1 tab to separate fields\n" <<
	"quit\t\t\texit this program, does not save\n" <<
	"help\t\t\tshow this message\n\n";
	
	output << "Licensed under the GNU GPL v3 or later\n";
}
