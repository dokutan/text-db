# variables
BIN_DIR = /usr/bin
CC = g++
CC_OPTIONS := -Wall -Wextra -O2

# version string
VERSION_STRING = "\"0.1α\""

# compile
build: text-db.o textdb.o dbitem.o utils.o frontend.o
	$(CC) *.o -o text-db $(CC_OPTIONS)

install:
	cp ./text-db $(BIN_DIR)/text-db

uninstall:
	rm $(BIN_DIR)/text-db

clean:
	rm  *.o ./text-db

# individual files
text-db.o:
	$(CC) -c text-db.cpp $(CC_OPTIONS) -D VERSION_STRING=$(VERSION_STRING)

textdb.o:
	$(CC) -c include/textdb.cpp $(CC_OPTIONS)

dbitem.o:
	$(CC) -c include/dbitem.cpp $(CC_OPTIONS)

utils.o:
	$(CC) -c include/utils.cpp $(CC_OPTIONS)

frontend.o:
	$(CC) -c include/frontend.cpp $(CC_OPTIONS)
