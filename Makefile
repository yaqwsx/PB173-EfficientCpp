ex1: ex1.cpp ex1.hpp
	c++ -O3 -std=c++14 ex1.cpp -o $@
ex1-test: ex1-test.cpp ex1.hpp
	c++ -std=c++14 -g ex1-test.cpp -o $@
hw1: hw1.cpp hw1.hpp
	c++ -O3 -std=c++1z hw1.cpp -o $@
hw1-test: hw1-test.cpp hw1.hpp
	c++ -std=c++1z -g hw1-test.cpp -o $@
hw2: hw2.cpp hw2.hpp
	c++ -O3 -Ibricks -g -std=c++1z hw2.cpp -o $@
hw2-test: hw2-test.cpp hw2.hpp
	c++ -O2 -g -std=c++1z hw2-test.cpp -o $@
hw3-test: hw2-test.cpp hw2.hpp
	c++ -std=c++1z -g hw1-test.cpp -o $@
ex2: ex2.cpp
	c++ -O2 -std=c++1z -g -Ibricks ex2.cpp -o $@
ex3: ex3.cpp
	c++ -O2 -std=c++1z -g -Ibricks ex3.cpp -o $@
hw3: hw3.cpp
	c++ -O2 -g -std=c++1z -Ibricks hw3.cpp -o $@
hw4: hw4.cpp
	c++ -O2 -g -std=c++1z -Ibricks hw4.cpp -o $@