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

#include <boost/program_options.hpp>

#include "backend.cpp"

// g++ text-db.cpp -o text-db -lboost_program_options

int main( int argc, char* argv[] ){
	
	// namespaces
	using namespace std;
	namespace po = boost::program_options;
	
	try{
	
	// declare commandline options: general
	po::options_description general_options_desc("General options");
	general_options_desc.add_options()
		("help,h", "Print this message")
		("file,f", po::value< string >()->required(), "Storage file");
	
	// declare commandline options: input
	po::options_description input_options_desc("Input options");
	input_options_desc.add_options()
		("add,a", po::value< vector<string> >()->multitoken(), "Add item, key or value")
		("copy,C", po::value< vector<string> >()->multitoken(), "Copy item")
		("delete,d", po::value< vector<string> >()->multitoken(), "Delete item or key");
	
	// declare commandline options: output
	po::options_description output_options_desc("Output options");
	output_options_desc.add_options()
		("show-all,S", "Show collection")
		("show,s", po::value< vector<string> >()->multitoken(), "Show item or key")
		("with,w", po::value< vector<string> >()->multitoken(), "Show items with key")
		("without,W", po::value< vector<string> >()->multitoken(), "Show items without key")
		("number,n", po::value< bool >()->default_value(true), "Show number of items")
		("color,c", po::value< bool >()->default_value(true), "Use colored output")
		("item-order,i", po::value< vector<string> >()->multitoken(), "Sort items in output")
		("item-order-file,I", po::value< vector<string> >()->multitoken(), "Sort items in output and file")
		("force-write,F", po::value< vector<string> >()->multitoken(), "Write to file even when no changes were made");
	
	// combine all options
	po::options_description all_options_desc("Allowed options");
	all_options_desc.add(general_options_desc).add(input_options_desc).add(output_options_desc);
	
	// declare positional commandline options
	po::positional_options_description po_desc;
	po_desc.add("file", 1);
	
	// parse commandline into variables_map
	po::variables_map var_map;
	po::store(po::command_line_parser( argc, argv ).
		options( all_options_desc ).positional( po_desc ).run(), var_map);
	
	
	// check for help option
	if( var_map.count("help") ){
		// print help
		cout << all_options_desc;
		cout << "\nSupported sort arguments:\n"
		" key\n key-reverse\n num-keys\n num-keys-reverse\n";
		return 0;
	}
	
	// check for correct commandline options
	po::notify(var_map);
	
	// main object
	plaintext_database db;
	
	// load from storage file
	string file = "";
	if( var_map.count("file") ){
		file = var_map["file"].as< string >();
		db.load_from_file( file );
	}
	bool write_file = false;
	
	// apply output options
	if( var_map.count("color") ){
		bool color = var_map["color"].as< bool >();
		db.set_color( color );
	}
	if( var_map.count("number") ){
		bool number = var_map["number"].as< bool >();
		db.set_show_number( number );
	}
	
	// delete item, key
	if( var_map.count("delete") ){
		
		vector< string > parameters = var_map["delete"].as< vector< string > >();
		
		if( parameters.size() == 1 ){
			// delete item
			db.delete_item( parameters[0] );
			write_file = true;
		} else if( parameters.size() == 2 ){
			// delete key from item
			db.delete_item_key( parameters[0], parameters[1] );
			write_file = true;
		} else{
			throw invalid_argument( "Wrong number of arguments" );
		}
		
	}
	// add item, key, value
	if( var_map.count("add") ){
		
		vector< string > parameters = var_map["add"].as< vector< string > >();
		
		if( parameters.size() == 1 ){
			// add item
			db.add_item( parameters[0] );
			write_file = true;
		} else if( parameters.size() == 2 ){
			// add item with key
			db.add_item_key( parameters[0], parameters[1] );
			write_file = true;
		} else if( parameters.size() == 3 ){
			// add item with key = value
			db.add_item_key( parameters[0], parameters[1], parameters[2] );
			write_file = true;
		} else{
			throw invalid_argument( "Wrong number of arguments" );
		}
		
	}
	// copy item
	if( var_map.count("copy") ){
		
		vector< string > parameters = var_map["copy"].as< vector< string > >();
		
		if( parameters.size() == 2 ){
			// copy item
			db.copy_item( parameters[0], parameters[1] );
			write_file = true;
		} else{
			throw invalid_argument( "Wrong number of arguments" );
		}
		
	}
	// sort output?
	if( var_map.count("item-order") ){
		
		vector< string > parameters = var_map["item-order"].as< vector< string > >();
		
		if( parameters.size() == 1 ){
			// copy item
			db.sort( parameters );
			db.set_cout_use_sorted( true );
		} else{
			throw invalid_argument( "Wrong number of arguments" );
		}
		
	}
	// sort output and file?
	if( var_map.count("item-order-file") ){
		
		vector< string > parameters = var_map["item-order-file"].as< vector< string > >();
		
		if( parameters.size() == 1 ){
			// copy item
			db.sort( parameters );
			db.set_cout_use_sorted( true );
			db.set_file_use_sorted( true );
		} else{
			throw invalid_argument( "Wrong number of arguments" );
		}
		
	}
	// show collection, item, key
	if( var_map.count("show-all") ){
		db.write_to_cout();
	}
	if( var_map.count("show") ){
		
		vector< string > parameters = var_map["show"].as< vector< string > >();
		
		if( parameters.size() == 1 ){
			// show item
			db.show_item( parameters[0] );
		} else if( parameters.size() == 2 ){
			// show key from item
			db.show_item_key( parameters[0], parameters[1] );
		} else{
			throw invalid_argument( "Wrong number of arguments" );
		}
		
	}
	// show items with key or key = value
	if( var_map.count("with") ){
		
		vector< string > parameters = var_map["with"].as< vector< string > >();
		
		if( parameters.size() == 1 ){
			// show items with key
			db.show_items_with_key( parameters[0] );
		} else if( parameters.size() == 2 ){
			// show items with key = value
			db.show_items_with_key_value( parameters[0], parameters[1] );
		} else{
			throw invalid_argument( "Wrong number of arguments" );
		}
		
	}
	// show items without key or key = value
	if( var_map.count("without") ){
		
		vector< string > parameters = var_map["without"].as< vector< string > >();
		
		if( parameters.size() == 1 ){
			// show items without key
			db.show_items_without_key( parameters[0] );
		} else if( parameters.size() == 2 ){
			// show items without key = value
			db.show_items_without_key_value( parameters[0], parameters[1] );
		} else{
			throw invalid_argument( "Wrong number of arguments" );
		}
		
	}
	// force writing to file
	if( var_map.count("force-write") ){
		write_file = true;
	}
	// write to file
	if( write_file ){
		db.write_to_file( file );
	}
	
	// error handling
	} catch( exception &e ){
		cout << "Error: " << e.what() << "\n";
		return 1;
	}
	
	return 0;
}
