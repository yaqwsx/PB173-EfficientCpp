#include "ex1.hpp"
#include <random>
#include <limits>
#include <chrono>
#include <iostream>
#include <string>
#include <cstring>

int randomInt() {
    static std::random_device r;
    static std::default_random_engine e1( r() );
    static std::uniform_int_distribution< int > uniform_dist(
        std::numeric_limits< int >::min(),
        std::numeric_limits< int >::max()
    );
    return uniform_dist( e1 );
}

template < typename Buffer >
void benchmark( int count, const char* name ) {
    using namespace std::chrono;

    Buffer buff;
    auto start = high_resolution_clock::now();
    for ( int i = 0; i != count; i++ )
        buff.insert( randomInt() );
    auto end = high_resolution_clock::now();
    std::cout << "Benchmark " << name << "\n";
    std::cout << "==========" << std::string( std::strlen( name ) , '=' ) << "\n";
    std::cout << "Took: " << duration_cast< milliseconds >( end - start ).count()
        << " ms\n";
    std::cout << std::endl;
}

int main() {
    const int count = 10'000'000;
    benchmark< BoundedSetBuffer< int, 5 > >( count, "Set<int, 5>" );
    benchmark< BoundedSetBuffer< int, 10 > >( count, "Set<int, 10>" );
    benchmark< BoundedSetBuffer< int, 10'000 > >( count, "Set<int, 10'000>" );

    benchmark< BoundedVectorBuffer< int, 5 > >( count, "Vector<int, 5>" );
    benchmark< BoundedVectorBuffer< int, 10 > >( count, "Vector<int, 10>" );
    benchmark< BoundedVectorBuffer< int, 10'000 > >( count, "Vector<int, 10'000>" );
}