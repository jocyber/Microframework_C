FLAGS := -std=c++20 -Wall -pedantic-errors -O2 -g -pthread -L/usr/include/mariadb/mysql -lmariadbclient
SOURCE := source_files
DEV := Dev_Folder
EXE := framework

all: $(SOURCE)/*.cpp $(DEV)/*.cpp
	g++ $(FLAGS) $^ -o $(EXE)

run:
	./$(EXE)

clean:
	rm $(EXE)

start:
	./structure.sh

open:
	vim -p $(SOURCE)/*.cpp $(SOURCE)/*.h

commit:
	./commit.sh
