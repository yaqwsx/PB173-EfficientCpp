#include "hw1.hpp"
#include <cassert>
#include <iostream>
#include <unordered_set>

template < template< typename > class T >
void test() {
    T< int > x;
    for ( int i = 0; i != 2000; i++ ) {
        x.insert( i );
    }
    for ( int i = 0; i != 2000; i++ ) {
        assert( x.find( i ) != nullptr );
        assert( *x.find( i ) == i );
    }

    T< int > y;
    std::unordered_set< int > z;
    for ( int i = 0; i != 20000; i++ ) {
        int value = randomInt();
        y.insert( value );
        z.insert( value );
    }

    assert( y._size == z.size() );

    for( int value : z ) {
        assert( y.find( value ) );
    }
}

int main() {
    std::cout << "Testing BucketSet\n";
    test< BucketSet >();
    std::cout << "Test LinearSet\n";
    test< LinearSet >();

    std::cout << "“Doctors are not required.” — Cyberman (S10E12 - The Doctor Falls)\n";
}