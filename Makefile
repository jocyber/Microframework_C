FLAGS := -std=c++20 -Wall -pedantic-errors -O2 -g -pthread
SOURCE := source_files
EXE := framework

all: $(SOURCE)/*.cpp
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
