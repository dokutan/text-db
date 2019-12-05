BIN_DIR = /usr/bin

install:
	g++ text-db.cpp -o text-db -lboost_program_options && \
	cp ./text-db $(BIN_DIR)/text-db

uninstall:
	rm $(BIN_DIR)/text-db

clean:
	rm ./text-db
