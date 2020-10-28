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
		command_help( output );
	
	// quit
	else if( std::regex_match( input, std::regex("(quit|close|exit)") ) )
	{
		exit(0);
	}
	
	// print everything
	else if( std::regex_match( input, std::regex("(ls|print|search)") ) )
	{
		db.print( output, (options["color"] == "on") );
		return;
	}
	
	// search by key
	else if( std::regex_match( input, std::regex("(ls|print|search)[[:s:]]([^\t]+\t?)+") ) )
	{
		std::string term_string = std::regex_replace( input, std::regex("(ls|print|search)[[:s:]]"), "" );
		
		command_search_by_key( term_string, db, output, (options["color"] == "on"), (options["regex"] == "on") );
		return;
	}
	
	// search by key and value
	else if( std::regex_match( input, std::regex("(ls|print|search)[[:s:]].+\t\t.+") ) )
	{
		std::string term_string = std::regex_replace( input, std::regex("(ls|print|search)[[:s:]]"), "" );
		std::string key_string = std::regex_replace( term_string, std::regex("\t\t.*"), "" ); // keys
		std::string value_string = std::regex_replace( term_string, std::regex(".*\t\t"), "" ); // values
		
		command_search_by_key_value( key_string, value_string, db, output, (options["color"] == "on"), (options["regex"] == "on") );
		return;
	}
	
	// clear database
	else if( std::regex_match( input, std::regex("clear") ) )
	{
		db.items().clear();
		output << "Deleted everything\n";
		return;
	}
	
	// count items
	else if( std::regex_match( input, std::regex("(count|size)") ) )
	{
		command_count( output, db );
		return;
	}
	
	// list all options
	else if( std::regex_match( input, std::regex("option") ) )
	{
		for( auto o : options )
			output << o.first << "\t" << o.second << "\n";
		
		return;
	}
	
	// get option
	else if( std::regex_match( input, std::regex("option[[:s:]][^[:s:]]+") ) )
	{
		std::string option = std::regex_replace( input, std::regex("option[[:s:]]"), "" );
		
		if( options.find( option ) != options.end() )
			output << option << "\t" << options[option] << "\n";
		
		return;
	}
	
	// set option
	else if( std::regex_match( input, std::regex("option[[:s:]][^[:s:]]+[[:s:]][^[:s:]]+") ) )
	{
		std::string option = std::regex_replace( std::regex_replace( input, std::regex("option[[:s:]]"), "" ), std::regex("[[:s:]][^[:s:]]+"), "" );
		std::string value = std::regex_replace( input, std::regex("option[[:s:]][^[:s:]]+[[:s:]]"), "" );
		
		options[option] = value;
		return;
	}
	
	// show opened file
	else if( std::regex_match( input, std::regex("(open|load)") ) )
	{
		if( options.find( "file" ) != options.end() )
			output << options["file"] << "\n";
		
		return;
	}
	
	// open file
	else if( std::regex_match( input, std::regex("(open|load)[[:s:]].+") ) )
	{
		std::string filename = std::regex_replace( input, std::regex("(open|load)[[:s:]]"), "" );
		
		command_load_file( filename, db, options, output );
		return;
	}
	
	// save to currently opened file
	else if( std::regex_match( input, std::regex("save") ) )
	{
		if( options.find( "file" ) != options.end() )
			command_save_file( options["file"], db, output );
		else
			output << "Please specify a file.\n";
		
		return;
	}
	
	// save to specified file
	else if( std::regex_match( input, std::regex("save[[:s:]].+") ) )
	{
		std::string filename = std::regex_replace( input, std::regex("save[[:s:]]"), "" );
		
		command_save_file( filename, db, output );
		return;
	}
	
	// add values
	else if( std::regex_match( input, std::regex("(add-value|touch)[[:s:]].+\t\t.+") ) )
	{
		std::string term_string = std::regex_replace( input, std::regex("(add-value|touch)[[:s:]]"), "" );
		std::string key_string = std::regex_replace( term_string, std::regex("\t\t.*"), "" );
		std::string value_string = std::regex_replace( term_string, std::regex(".*\t\t"), "" );
		
		command_add_values( key_string, value_string, db, output, (options["regex"] == "on") );
		return;
	}
	
	// add keys
	else if( std::regex_match( input, std::regex("(add-item|mkdir)[[:s:]]([^\t]+\t?)+") ) )
	{
		// search terms
		std::string term_string = std::regex_replace( input, std::regex("(add-item|mkdir)[[:s:]]"), "" );
		textdb::keys new_keys({});
		textdb::string_to_vector( term_string, new_keys, db.delimiter() );
		
		while( new_keys.size() >= 1 )
		{
			db.items().try_emplace( new_keys, textdb::values() );
			new_keys.pop_back();
		}
		return;
	}
	
	// add subkeys to existing keys
	else if( std::regex_match( input, std::regex("(add-item|mkdir)[[:s:]]([^\t]+\t?)+\t\t([^\t]+\t?)+") ) )
	{
		std::string term_string = std::regex_replace( input, std::regex("(add-item|mkdir)[[:s:]]"), "" );
		std::string key_string = std::regex_replace( term_string, std::regex("\t\t.*"), "" );
		std::string key_new_string = std::regex_replace( term_string, std::regex(".*\t\t"), "" );
		
		command_add_keys( key_string, key_new_string, db, output, (options["regex"] == "on") );
		return;
	}
	
	// delete values
	else if( std::regex_match( input, std::regex("(rm|delete)[[:s:]]([^\t]+\t?)+\t\t([^\t]+\t?)+") ) )
	{
		std::string term_string = std::regex_replace( input, std::regex("(rm|delete)[[:s:]]"), "" );
		std::string key_string = std::regex_replace( term_string, std::regex("\t\t.*"), "" );
		std::string value_string = std::regex_replace( term_string, std::regex(".*\t\t"), "" );
		
		command_delete_values( key_string, value_string, db, output, (options["regex"] == "on") );
		return;
	}
	
	// delete keys
	else if( std::regex_match( input, std::regex("(rm|delete)[[:s:]]([^\t]+\t?)+") ) )
	{
		std::string key_string = std::regex_replace( input, std::regex("(rm|delete)[[:s:]]"), "" );
		
		command_delete_keys( key_string, db, output, (options["regex"] == "on") );
		return;
	}
	
	// move/rename keys
	else if( std::regex_match( input, std::regex("(mv|rename)[[:s:]]([^\t]+\t?)+\t\t([^\t]+\t?)+") ) )
	{
		std::string term_string = std::regex_replace( input, std::regex("(mv|rename)[[:s:]]"), "" );
		std::string keys_old = std::regex_replace( term_string, std::regex("\t\t.*"), "" );
		std::string keys_new = std::regex_replace( term_string, std::regex(".*\t\t"), "" );
		
		command_rename_keys( keys_old, keys_new, db, output, (options["regex"] == "on") );
		return;
	}
	
	
	// copy keys
	else if( std::regex_match( input, std::regex("(cp)[[:s:]]([^\t]+\t?)+\t\t([^\t]+\t?)+") ) )
	{
		std::string term_string = std::regex_replace( input, std::regex("(cp)[[:s:]]"), "" );
		std::string keys_old = std::regex_replace( term_string, std::regex("\t\t.*"), "" );
		std::string keys_new = std::regex_replace( term_string, std::regex(".*\t\t"), "" );
		
		command_copy_keys( keys_old, keys_new, db, output, (options["regex"] == "on") );
		return;
	}
	
	// get values
	else if( std::regex_match( input, std::regex("get[[:s:]]([^\t]+\t?)+") ) )
	{
		std::string key_string = std::regex_replace( input, std::regex("get[[:s:]]"), "" );
		
		command_show_values( key_string, db, output, (options["regex"] == "on") );
		return;
	}
	
	// export to tsv
	else if( std::regex_match( input, std::regex("export[[:s:]]+tsv") ) )
	{
		db.to_tsv( output );
		return;
	}
	
	// export to graphviz
	/* TODO! Disabled for now, key elements with the same name are not handled
	else if( std::regex_match( input, std::regex("export[[:s:]]+gv") ) )
	{
		db.to_graphviz( std::cout );
		return;
	}
	*/
	
	else
		output << "Unknown command or invalid arguments, type help for a list of available commands\n";
	
}

void command_help( std::ostream& output )
{

output << 
R"(Usage:
text-db FILE [COMMAND]  load file, optionally run command
text-db - [COMMAND]     load database from stdin, optionally run command
text-db --help          show this message

Available commands:
help
quit|close|exit
open|load
open|load [file]
save
save [file]
ls|print|search
ls|print|search [keys]
ls|print|search [keys] [values]
count|size
add-value [keys] [values]
add-item|mkdir [keys]
add-item|mkdir [keys] [subkeys]
rm|delete [keys]
rm|delete [keys] [values]
clear
mv|rename [source keys] [dest keys]
cp [source keys] [dest keys]
get [keys]
option
option [option]
option [option] [value]

Use a single tab to separate fields in an argument, use two tabs to
separate between arguments, e.g.:
key1  <1 tab>  key2  <2 tabs>  value1  <1 tab>  value2

Licensed under the GNU GPL v3 or later
)";

}

void command_count( std::ostream& output, textdb& db )
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

void command_search_by_key( std::string& keys, textdb& db, std::ostream& output, bool use_color, bool use_regex )
{
	try
	{
		// search terms
		std::vector< std::string > terms({});
		textdb::string_to_vector( keys, terms, db.delimiter() );
		
		// holds the first element of the result keys
		std::set< std::string > results;
		
		// perform search
		for( auto& item : db.items() )
		{
			if( use_regex ? textdb::compare_vectors_regex_exact( item.first, terms ) : textdb::compare_vectors_exact( item.first, terms ) )
				results.emplace( item.first.front() );
		}
		
		// print results
		for( auto& r : results )
			db.print( output, use_color, textdb::keys({r}) );
		
	}
	catch( std::exception& e )
	{
		output << e.what() << "\n";
		return;
	}
}

void command_search_by_key_value( std::string& keys, std::string& values, textdb& db, std::ostream& output, bool use_color, bool use_regex )
{
	try
	{
		// search terms
		std::vector< std::string > key_terms({}), value_terms({});
		textdb::string_to_vector( keys, key_terms, db.delimiter() );
		textdb::string_to_vector( values, value_terms, db.delimiter() );
		
		// holds the first element of the result keys
		std::set< std::string > results;
		
		// perform search
		for( auto& item : db.items() )
		{
			
			// search by key
			if( use_regex ? textdb::compare_vectors_regex_exact( item.first, key_terms ) : textdb::compare_vectors_exact( item.first, key_terms ) )
			{
				
				// check values
				bool values_match = false;
				
				// iterate over value search terms
				for( auto value_term : value_terms )
				{
					// iterate over item values
					for( auto value : item.second )
					{
						// check value
						if( use_regex ? std::regex_match( value, std::regex(value_term) ) : (value == value_term) )
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
			db.print( output, use_color, textdb::keys({r}) );
		
	}
	catch( std::exception& e )
	{
		output << e.what() << "\n";
		return;
	}
}

void command_load_file( std::string& filename, textdb& db, std::map< std::string, std::string >& options, std::ostream& output )
{
	std::ifstream infile( filename );
	
	if( !infile.is_open() )
	{
		output << "Could not open " << filename << "\n";
		return;
	}
	
	options["file"] = filename;
	db.items().clear();
	db.load( infile );
	infile.close();
}

void command_save_file( std::string& filename, textdb& db, std::ostream& output )
{
	std::ofstream outfile( filename );
	
	if( !outfile.is_open() )
	{
		output << "Could not open " << filename << "\n";
		return;
	}
	
	db.print( outfile, false );
	outfile.close();
}

void command_add_values( std::string& key_string, std::string& value_string, textdb& db, std::ostream& output, bool use_regex )
{
	try
	{
		// search terms
		std::vector< std::string > key_terms({}), value_terms({});
		textdb::string_to_vector( key_string, key_terms, db.delimiter() );
		textdb::string_to_vector( value_string, value_terms, db.delimiter() );
		
		// perform search
		for( auto& item : db.items() )
		{
			if( use_regex ? textdb::compare_vectors_regex_exact( item.first, key_terms ) : textdb::compare_vectors_exact( item.first, key_terms ) )
			{
				// store values: iterate over value terms
				for( auto value_term : value_terms )
				{
					if( std::find( item.second.begin(), item.second.end(), value_term ) == item.second.end() )
						item.second.push_back( value_term );
				}
			}
		}
	}
	catch( std::exception& e )
	{
		output << e.what() << "\n";
		return;
	}
}

void command_add_keys( std::string& key_string, std::string& key_new_string, textdb& db, std::ostream& output, bool use_regex )
{
	try
	{
		std::vector< std::string > key_terms({}), new_keys({});
		textdb::string_to_vector( key_string, key_terms, db.delimiter() );
		textdb::string_to_vector( key_new_string, new_keys, db.delimiter() );
		
		// perform search
		for( auto& item : db.items() )
		{
			if( use_regex ? textdb::compare_vectors_regex_exact( item.first, key_terms ) : textdb::compare_vectors_exact( item.first, key_terms ) )
			{
				// store new keys
				for( auto new_key : new_keys )
				{
					textdb::keys new_path = item.first;
					new_path.push_back( new_key );
					
					db.items().emplace( new_path, textdb::values() ); //?
				}
				
			}
		}
	}
	catch( std::exception& e )
	{
		output << e.what() << "\n";
		return;
	}
}

void command_delete_keys( std::string& key_string, textdb& db, std::ostream& output, bool use_regex )
{
	try
	{
		// search terms
		std::vector< std::string > deletion_keys({});
		textdb::string_to_vector( key_string, deletion_keys, db.delimiter() );
		
		std::vector< textdb::keys > results;
		
		// iterate over items
		for( auto& item : db.items() )
		{
			if( use_regex ? textdb::compare_vectors_regex( item.first, deletion_keys ) : textdb::compare_vectors( item.first, deletion_keys ) )
				results.push_back( item.first );
		}
		
		// delete results
		for( auto& r : results )
			db.items().erase( r );
		
	}
	catch( std::exception& e )
	{
		output << e.what() << "\n";
		return;
	}
}

void command_delete_values( std::string& key_string, std::string& value_string, textdb& db, std::ostream& output, bool use_regex )
{
	try
	{
		// search terms
		std::vector< std::string > key_terms({}), value_terms({});
		textdb::string_to_vector( key_string, key_terms, db.delimiter() );
		textdb::string_to_vector( value_string, value_terms, db.delimiter() );
		
		// perform search, iterate over items
		for( auto& item : db.items() )
		{
			std::vector< std::string > results;
			
			// if paths match
			if( use_regex ? textdb::compare_vectors_regex_exact( item.first, key_terms ) : textdb::compare_vectors_exact( item.first, key_terms ) )
			{
				// delete values: iterate over item values
				for( auto& value : item.second )
				{
					// iterate over item value terms
					for( auto& value_term : value_terms )
					{
						// check value, store in results if value matches
						if( use_regex ? std::regex_match( value, std::regex(value_term) ) : (value == value_term) )
							results.push_back( value );
						
					}
					
				}
				
			}
			
			// delete values stored in results
			for( auto& r : results )
				item.second.erase( std::find( item.second.begin(), item.second.end(), r ) );
			
		}
	}
	catch( std::exception& e )
	{
		output << e.what() << "\n";
		return;
	}
}

void command_rename_keys( std::string& keys_old, std::string& keys_new, textdb& db, std::ostream& output, bool use_regex )
{
	try
	{
		// search terms
		std::vector< std::string > old_key_terms({}), new_key_terms({});
		textdb::string_to_vector( keys_old, old_key_terms, db.delimiter() );
		textdb::string_to_vector( keys_new, new_key_terms, db.delimiter() );
		
		std::vector< textdb::keys > results_delete; // the old keys for deletion
		textdb results_add; // the new key-value pairs
		
		// delete already existing target
		command_delete_keys( keys_new, db, output, false );
		
		// iterate over items
		for( auto& item : db.items() )
		{
			if( use_regex ? textdb::compare_vectors_regex( item.first, old_key_terms ) : textdb::compare_vectors( item.first, old_key_terms ) )
			{
				results_delete.push_back( item.first );
				
				// build new path
				textdb::keys new_path = new_key_terms;
				for( size_t i = old_key_terms.size(); i < item.first.size(); i++ )
					new_path.push_back( item.first.at(i) );
				results_add.items().emplace( new_path, item.second );
				
				// create new parent items as required
				while( new_path.size() > 0 ){
					results_add.items().try_emplace( new_path, textdb::values() );
					new_path.pop_back();
				}
				
				
			}
		}
		
		// delete old items
		for( auto& r : results_delete )
			db.items().erase( r );
		
		// add new items
		// db.items().merge( results_add.items() );
		for( auto& r : results_add.items() )
			db.items().insert_or_assign( r.first, r.second );
		
	}
	catch( std::exception& e )
	{
		output << e.what() << "\n";
		return;
	}
}

void command_copy_keys( std::string& keys_old, std::string& keys_new, textdb& db, std::ostream& output, bool use_regex )
{
	try
	{
		// search terms
		std::vector< std::string > old_key_terms({}), new_key_terms({});
		textdb::string_to_vector( keys_old, old_key_terms, db.delimiter() );
		textdb::string_to_vector( keys_new, new_key_terms, db.delimiter() );
		
		textdb results_add; // the new key-value pairs
		
		// iterate over items
		for( auto& item : db.items() )
		{
			if( use_regex ? textdb::compare_vectors_regex( item.first, old_key_terms ) : textdb::compare_vectors( item.first, old_key_terms ) )
			{
				// build new path
				textdb::keys new_path = new_key_terms;
				for( size_t i = old_key_terms.size(); i < item.first.size(); i++ )
					new_path.push_back( item.first.at(i) );
				results_add.items().emplace( new_path, item.second );
				
				// create new parent items as required
				while( new_path.size() > 0 ){
					results_add.items().try_emplace( new_path, textdb::values() );
					new_path.pop_back();
				}
				
				
			}
		}
		
		// add new items
		// db.items().merge( results_add.items() );
		for( auto& r : results_add.items() )
			db.items().insert_or_assign( r.first, r.second );
		
	}
	catch( std::exception& e )
	{
		output << e.what() << "\n";
		return;
	}
}

void command_show_values( std::string& keys, textdb& db, std::ostream& output, bool use_regex )
{
	try
	{
		// search terms
		std::vector< std::string > deletion_keys({});
		textdb::string_to_vector( keys, deletion_keys, db.delimiter() );
		
		// iterate over items
		for( auto& item : db.items() )
		{
			if( use_regex ? textdb::compare_vectors_regex_exact( item.first, deletion_keys ) : textdb::compare_vectors_exact( item.first, deletion_keys ) ){
				
				size_t size = item.second.size();
				for( auto& value : item.second )
				{
					output << value << ((size > 1) ? "\t" : "");
					size--;
				}
				output << "\n";
			}
		}
	}
	catch( std::exception& e )
	{
		output << e.what() << "\n";
		return;
	}
}
