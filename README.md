# text-db
Plain text collection manager / database

## Data structure
The data this program can work with consists of a key-value pairs ordered as a tree. This is intended for keeping track of collections, but is not limited to this use. The data is stored in plain text so it can be easily edited manually or with other tools. The storage format is as follows (tabs as separators):
```
key1
	key1.1
	key1.2	value1
key2
key3	value3
	key3.1	value3.1
		key3.1.1	value3.1.1
```

## Installation
- Install a C++17 capable compiler
- Clone this repository
- Run
```
make
sudo make install
```

## Usage examples
Show usage information: ``text-db --help``

### Loading a collection
The program can be used interactively: ``text-db example``

The program can be used from the shell: ``text-db example ls``

A collection can be loaded from stdin: ``cat example | text-db - ls``

### Changing a collection 
Open the example with ``text-db example``, then type the following commands. It is important to use a single tab to separate fields in an argument and two tabs to
separate between arguments.

```
ls
ls key3
ls key(2|3)
cp key1		key4
rm key1	key1.*
add-value key4		value4
add-key key5	key5.1
save
exit
```

## Limitations
- The available commands and their arguments are not final.
- This is not a replacement for a real database, and not intended as such.

## License
GNU GPL v3 or later
