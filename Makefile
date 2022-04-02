FLAGS := -std=c++20 -Wall -pedantic-errors -O3 -g -pthread
SQL := `mariadb_config --cflags --libs`
SOURCE := source_files
DEV := Dev_Folder
EXE := framework

all: $(SOURCE)/*.cpp $(DEV)/*.cpp
	g++ $(FLAGS) $^ -o $(EXE) $(SQL)

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
