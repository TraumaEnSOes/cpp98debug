test: main.cpp cpp98debug.hpp
	g++-4.8 -g3 -ggdb3 -O0 -std=c++98 -Wall -Wextra -pedantic -o test main.cpp

.PHONY: clean
clean:
	rm -f test
