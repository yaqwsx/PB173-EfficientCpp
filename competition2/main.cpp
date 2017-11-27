#include <iostream>
#include <cstdint>
#include <cstdlib>

uint64_t naive_prime( int n ) {
    for ( uint64_t i = 2; true; i++) {
        if ( i != 2 && i % 2 == 0 )
            continue;
        bool valid = true;
        for ( uint64_t j = 2; valid && j != i; j++ ) {
            if ( i % j == 0 )
                valid = false;
        }
        if ( valid ) n--;
        if ( n == 0 )
            return i;
    }
    return -1;
}

int main( int argc, char **argv ) {
    if ( argc != 2 ) {
        std::cerr << "Oh, geez, Rick!" << std::endl;
        return 1;
    }
    int n = std::atoi( argv[ 1 ] );
    std::cout << naive_prime( n ) << std::endl;
}