FLAGS := -Wall -pedantic-errors -O2 -g

run: server

%: %.cpp httpReq.cpp
	g++ $(FLAGS) $^ -o framework
