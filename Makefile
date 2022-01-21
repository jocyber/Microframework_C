FLAGS := -Wall -pedantic-errors -O2 -g
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

