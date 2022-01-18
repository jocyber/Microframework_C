FLAGS := -Wall -pedantic-errors -O2 -g
SOURCE := source_files

all: server

%: $(SOURCE)/%.cpp $(SOURCE)/httpReq.cpp
	g++ $(FLAGS) $^ -o $(SOURCE)/framework

run:
	$(SOURCE)/framework

clean:
	rm $(SOURCE)/framework

start:
	./structure.sh

