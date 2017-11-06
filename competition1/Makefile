HPP != ls *.hpp
CXXFLAGS = -Ibricks -g -std=c++14

check: check.cpp $(HPP)
	$(CXX) $(CXXFLAGS) -o check check.cpp
	./check

bench: bench.cpp $(HPP)
	$(CXX) $(CXXFLAGS) -O2 -o bench bench.cpp
	rm -f benchmark.log
	./bench category:charset > bench.gp
	gnuplot < bench.gp > bench.pdf

personalize:
	mv ref.hpp $(I).hpp
	sed -e s,Ref,$(I),g -e s,ref.hpp,$(I).hpp, -i $(I).hpp check.cpp bench.cpp

clean:
	rm -f check bench benchmark.log bench.pdf bench.gp
