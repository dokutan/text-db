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

class plaintext_database{
	
	private:
	
	std::map< std::string, std::map< std::string, std::string> >
		_collection;
	bool _use_color=true;
	bool _show_number=true;
	int _information_level=1;
	
	public:
	
	int load_from_file( std::string path );
	int write_to_file( std::string path );
	
	int delete_item( std::string item );
	int delete_item_key( std::string item, std::string key  );
	
	int add_item( std::string item );
	int add_item_key( std::string item, std::string key,
		std::string value );
	
	int write_to_cout();
	int show_item( std::string item );
	int show_item_key( std::string item, std::string key );
	int show_items_with_key( std::string key );
	int show_items_with_key_value( std::string key, std::string value );
	int show_items_without_key( std::string key );
	int show_items_without_key_value( std::string key, std::string value );
	
	int set_color( bool use_color );
	int set_show_number( bool show_number );
	
	int copy_item( std::string source, std::string dest );
};

// load from file
int plaintext_database::load_from_file( std::string path ){
	
	using namespace std;
	
	ifstream collection_file;
	collection_file.open( path );
	
	if( !collection_file.is_open() ){
		throw invalid_argument( "Couldn't open "+path );
		return 1;
	}
	
	map< string, string> empty_map;
	string current_item;
	size_t position = 0;
	for( string line; getline(collection_file, line); ){
		
		if( line.length() != 0 ){
			
			if( line.front() != '\t' ){ //item
				
				//insert new item
				_collection.insert( std::pair< string,
					map< string, string> >(line, empty_map) );
				current_item = line;
				
			} else{ // key-value pair
				
				//remove first \t
				line.erase(0, 1);
				position = line.find("\t");
				if( position != string::npos ){
					_collection[current_item][line.substr(0, position)]
						= line.substr(position+1);
				} else{
					_collection[current_item][line] = "";
				}
				
			}
		}
	}
	
	collection_file.close();
	return 0;
}

// write everything to file
int plaintext_database::write_to_file( std::string path ){
	
	using namespace std;
	
	ofstream collection_file;
	collection_file.open( path );
	
	if( !collection_file.is_open() ){
		throw invalid_argument( "Couldn't open "+path );
		return 1;
	}
	
	for( auto i : _collection ){
		
		collection_file << i.first << "\n";
		for( auto j : i.second ){
			collection_file << "\t" << j.first <<
				"\t" << j.second << "\n";
		}
		
	}
	
	collection_file.close();
	return 0;
}

// write everything to cout
int plaintext_database::write_to_cout(){
	
	using namespace std;
	
	for( auto i : _collection ){
		
		_use_color ?
			cout << "\e[35;1m" << i.first << "\e[0m\n" :
			cout << i.first << "\n";
		for( auto j : i.second ){
			_use_color ?
				cout << "\t\e[1m" << j.first << "\e[0m\t" << j.second << "\n" :
				cout << "\t" << j.first << "\t" << j.second << "\n";
		}
		
	}
	if( _show_number ){
	_use_color ?
		cout << "\e[36;1mNumber of items:\e[0m " << _collection.size() << "\n" :
		cout << "Number of items: " << _collection.size() << "\n" ;
	}
	
	return 0;
}

// delete item
int plaintext_database::delete_item( std::string item ){
	return _collection.erase( item );
}

// delete key from item
int plaintext_database::delete_item_key( std::string item,
	std::string key  ){
	return _collection[item].erase( key );
}

// add item
int plaintext_database::add_item( std::string item ){
	using namespace std;
	map< string, string> empty_map;
	_collection.insert( std::pair< string,
		map< string, string> >(item, empty_map) );
	return 0;
}

// add key (and item)
int plaintext_database::add_item_key( std::string item,
	std::string key, std::string value="" ){
	_collection[item][key] = value;
	return 0;	
}

// print single item
int plaintext_database::show_item( std::string item ){
	
	if( _collection.find(item) != _collection.end() ){
		_use_color ?
			std::cout << "\e[35;1m" << item << "\e[0m\n" :
			std::cout << item << "\n";
		for( auto i : _collection[item] ){
			_use_color ?
				std::cout << "\t\e[1m" << i.first << "\e[0m\t" << i.second << "\n" :
				std::cout << "\t" << i.first << "\t" << i.second << "\n";
		}
	} else{
		throw std::invalid_argument( "Item "+item+" doesn't exist" );
		return 1;
	}
	return 0;	
}

// show value of key in item
int plaintext_database::show_item_key( std::string item,
	std::string key ){
		
	if( _collection.find(item) != _collection.end() ){
		if( _collection[item].find(key) != _collection[item].end() ){
			_use_color ?
				std::cout << "\e[35;1m" << item << "\e[0m\n" :
				std::cout << item << "\n";
			_use_color ?
				std::cout << "\t\e[1m" << key << "\e[0m\t" << _collection[item][key] << "\n" :
				std::cout << "\t" << key << "\t" << _collection[item][key] << "\n";
		} else{
			throw std::invalid_argument( "Key "+key+" doesn't exist" );
		}
	} else{
		throw std::invalid_argument( "Item "+item+" doesn't exist" );
		return 1;
	}
	return 0;	
}

// show all items that have key
int plaintext_database::show_items_with_key( std::string key ){
	using namespace std;
	
	int number=0;
	
	for( auto i : _collection ){
		if( i.second.find( key ) != i.second.end() ){
			show_item( i.first );
			number++;
		}
	}
	
	if( _show_number ){
	_use_color ?
		cout << "\e[36;1mNumber of items:\e[0m " << number << "\n" :
		cout << "Number of items: " << number << "\n" ;
	}
	return 0;	
}

// show all items that have key = value
int plaintext_database::show_items_with_key_value( std::string key,
	std::string value ){
	using namespace std;
	
	int number=0;
	
	for( auto i : _collection ){
		if( ( i.second.find( key ) != i.second.end() ) &&
			( i.second[key] == value ) ){
			show_item( i.first );
			number++;
		}
	}
	
	if( _show_number ){
	_use_color ?
		cout << "\e[36;1mNumber of items:\e[0m " << number << "\n" :
		cout << "Number of items: " << number << "\n" ;
	}
	return 0;	
}

// show all items that don't have key
int plaintext_database::show_items_without_key( std::string key ){
	using namespace std;
	
	int number=0;
	
	for( auto i : _collection ){
		if( i.second.find( key ) == i.second.end() ){
			show_item( i.first );
			number++;
		}
	}
	
	if( _show_number ){
	_use_color ?
		cout << "\e[36;1mNumber of items:\e[0m " << number << "\n" :
		cout << "Number of items: " << number << "\n" ;
	}
	return 0;	
}

// show all items that don't have key = value
int plaintext_database::show_items_without_key_value( std::string key,
	std::string value ){
	using namespace std;
	
	int number=0;
	
	for( auto i : _collection ){
		if( ( i.second.find( key ) != i.second.end() ) &&
			( i.second[key] != value ) ){
			show_item( i.first );
			number++;
		}
	}
	
	if( _show_number ){
	_use_color ?
		cout << "\e[36;1mNumber of items:\e[0m " << number << "\n" :
		cout << "Number of items: " << number << "\n" ;
	}
	return 0;	
}

// setter for _use_color
int plaintext_database::set_color( bool use_color ){
	_use_color = use_color;
	return 0;
}

// setter for _show_number
int plaintext_database::set_show_number( bool show_number ){
	_show_number = show_number;
	return 0;
}

// copy item
int plaintext_database::copy_item( std::string source, std::string dest ){
	
	if( _collection.find(source) != _collection.end() ){
		_collection[dest] = _collection[source];
	} else{
		throw std::invalid_argument( "Item "+source+" doesn't exist" );
		return 1;
	}
	
	return 0;
}
