ex1: ex1.cpp ex1.hpp
	c++ -std=c++14 -g ex1.cpp -o $@
ex1-test: ex1-test.cpp ex1.hpp
	c++ -std=c++14 -g ex1-test.cpp -o $@