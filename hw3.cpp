#include <vector>
#include <cassert>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>

template <typename T >
struct Matrix {
    template < typename Gen >
    Matrix( int n, int m, Gen g ) {
        _data.reserve( n );
        std::generate_n( std::back_inserter( _data ), n, [&]{
            std::vector< T > row;
            row.reserve( m );
            std::generate_n( std::back_inserter( row ), m, g );
            return row;
        } );
    }

    Matrix( int n, int m ) : Matrix( n, m, []{ return 0; } ) {}

    std::vector< std::vector< T > > _data;
};

template < typename T >
Matrix< T > multiplyNaive( const Matrix< T >& a, const Matrix< T >& b ) {
    assert( a._data[ 0 ].size() == b._data.size() );
    int n = a._data.size();
    int m = b._data[ 0 ].size();
    int l = b._data.size();
    Matrix< T > res( n, m );
    for( int i = 0; i != n; i++ ) {
        for( int j = 0; j != n; j++ ) {
            for ( int k = 0; k != l; k++ )
                res._data[ i ][ j ] += a._data[ i ][ k ] * b._data[ k ][ j ];
        }
    }
    return res;
}

template < typename Fun >
void test( Fun mul ) {
    Matrix< int > A( 2, 3 );
    Matrix< int > B( 3, 2 );
    for ( int i = 0; i != 2; i++ )
        for ( int j = 0; j != 3; j++ )
            A._data[ i ][ j ] = i * 3 + j + 1;
    for ( int i = 0; i != 3; i++ )
        for ( int j = 0; j != 2; j++ )
            B._data[ i ][ j ] = i * 2 + j + 1;

    auto res = mul( A, B );
    assert( res._data[ 0 ][ 0 ] == 22 );
    assert( res._data[ 0 ][ 1 ] == 28 );
    assert( res._data[ 1 ][ 0 ] == 49 );
    assert( res._data[ 1 ][ 1 ] == 64 );
}

template < typename Fun >
void benchmark( const std::string& name, Fun mul ) {
    using namespace std::chrono;

    std::mt19937 rand{ 42 };
    std::uniform_real_distribution<> dist( 1.0, 2.0 );

    const int size = 2048;
    auto gen = [&]{ return dist( rand ); };
    Matrix< float > A( size, size, gen );
    Matrix< float > B( size, size, gen );

    auto start = high_resolution_clock::now();

    auto res = mul( A, B );

    auto end = high_resolution_clock::now();
    std::cout << "Benchmark " << name << "\n";
    std::cout << "==========" << std::string( name.size() , '=' ) << "\n";
    std::cout << "Took: " << duration_cast< milliseconds >( end - start ).count()
        << " ms\n";
    std::cout << std::endl;
}

int main() {
    test( multiplyNaive< int > );

    benchmark( "naive", multiplyNaive< float > );
}