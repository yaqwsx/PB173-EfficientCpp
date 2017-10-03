#include "hw1.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include <unordered_set>

std::vector< int >& samples() {
    static std::vector< int > _samples;
    if ( _samples.empty() ) {
        for ( int i = 0; i != 10'000'000; i++ )
            _samples.push_back( randomInt() );
    }
    return _samples;
}

template < typename Set >
void benchmark( const char* name ) {
    using namespace std::chrono;

    Set x;
    auto start = high_resolution_clock::now();
    for ( int value : samples() )
        x.insert( value );
    for ( int value : samples() )
        x.find( value );
    auto end = high_resolution_clock::now();
    std::cout << "Benchmark " << name << "\n";
    std::cout << "==========" << std::string( std::strlen( name ) , '=' ) << "\n";
    std::cout << "Took: " << duration_cast< milliseconds >( end - start ).count()
        << " ms\n";
    std::cout << std::endl;
}

int main() {
    benchmark< BucketSet< int > >( "BucketSet" );
    benchmark< LinearSet< int > >( "LinearSet" );
    benchmark< std::set< int > >( "std::set" );
    benchmark< std::unordered_set< int > >( "std::unordered_set" );
}