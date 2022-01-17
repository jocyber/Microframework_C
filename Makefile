FLAGS := -Wall -pedantic-errors -O2 -g

all: server

%: %.cpp httpReq.cpp
	g++ $(FLAGS) $^ -o framework

run:
	./framework

clean:
	rm framework

