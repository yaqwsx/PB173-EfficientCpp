ex1: ex1.cpp ex1.hpp
	c++ -O3 -std=c++14 ex1.cpp -o $@
ex1-test: ex1-test.cpp ex1.hpp
	c++ -std=c++14 -g ex1-test.cpp -o $@
hw1: hw1.cpp hw1.hpp
	c++ -O3 -std=c++1z hw1.cpp -o $@
hw1-test: hw1-test.cpp hw1.hpp
	c++ -std=c++1z -g hw1-test.cpp -o $@