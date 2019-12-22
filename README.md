# text-db
Plain text collection manager / database

## Data structure
The data this program can work with consists of items with associated keys. Keys can have a value. Items, keys and values are strings. This is intended for keeping track of collections, but is not limited to this use. The data is stored in plain text so it can be easily edited manually or with other tools. The storage format is as follows (only tabs as separators are supported):
```
item1
	key1
	key2	value
item2
item3
	key1	value
```

## Installation
- Install boost libraries
- Clone the repository
- Run
```
sudo make install
```

## Usage examples
- Create an empty file for the collection
```
touch ⟨file⟩
```
- Show all options
```
text-db --help
```
- Add an item
```
text-db ⟨file⟩ --add ⟨item⟩
```
- Add a key
```
text-db ⟨file⟩ --add ⟨item⟩ ⟨key⟩
```
- Add a key with value associated
```
text-db ⟨file⟩ --add ⟨item⟩ ⟨key⟩ ⟨value⟩
```
- Delete an item
```
text-db ⟨file⟩ --delete ⟨item⟩
```
- Delete a key from an item
```
text-db ⟨file⟩ --delete ⟨item⟩ ⟨key⟩
```
- Show a single item
```
text-db ⟨file⟩ --show ⟨item⟩
```
- Show all items with a particular key
```
text-db ⟨file⟩ --with ⟨key⟩
```
- Show all items with a particular key-value pair
```
text-db ⟨file⟩ --with ⟨key⟩ ⟨value⟩
```
- Show all items sorted by number of keys
```
text-db ⟨file⟩ --show-all --item-order num-keys
```

## Limitations
- When adding or deleting in a collection the whole file gets rewritten to keep the structure intact.
- This is not a replacement for a real database, and not intended as such.

## License
GNU GPL v3 or later
