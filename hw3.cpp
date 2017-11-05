#include <vector>
#include <cassert>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>

#define BRICK_BENCHMARK_REG
#define BRICK_BENCHMARK_MAIN
#include <brick-benchmark>

template <typename T >
struct Matrix1D {
    template < typename Gen >
    Matrix1D( int n, int m, Gen g ): _n( n ), _m( m ) {
        _data.reserve( m * n );
        std::generate_n( std::back_inserter( _data ), m * n, g );
    }

    Matrix1D( int n, int m ) : Matrix1D( n, m, []{ return 0; } ) {}

    Matrix1D(): _n( 0 ), _m( 0 ) {};

    struct _Idx {
        Matrix1D& _self;
        int n;
        T& operator[]( int m ) {
            return _self._data[ n * _self._m + m ];
        }
    };

    struct _CIdx {
        const Matrix1D& _self;
        int n;
        const T& operator[]( int m ) {
            return _self._data[ n * _self. _m + m ];
        }
    };

    _Idx operator[]( int n ) { return { *this, n }; }
    _CIdx operator[]( int n ) const { return { *this, n }; }


    std::vector< T > _data;
    int _n, _m;
};

template <typename T >
struct Matrix2D {
    template < typename Gen >
    Matrix2D( int n, int m, Gen g ) {
        _data.reserve( n );
        std::generate_n( std::back_inserter( _data ), n, [&]{
            std::vector< T > row;
            row.reserve( m );
            std::generate_n( std::back_inserter( row ), m, g );
            return row;
        } );
        _n = n;
        _m = m;
    }

    Matrix2D( int n, int m ) : Matrix2D( n, m, []{ return 0; } ) {}

    Matrix2D(): _n( 0 ), _m( 0 ) {};

    std::vector< T >& operator[]( int x ){ return _data[ x ]; }
    const std::vector< T >& operator[]( int x ) const { return _data[ x ]; }

    std::vector< std::vector< T > > _data;
    int _n, _m;
};

template < typename T >
using Matrix = Matrix1D< T >;

template < typename T >
Matrix< T > multiplyNaive( const Matrix< T >& a, const Matrix< T >& b ) {
    assert( a._m == b._n );
    int n = a._n;
    int m = b._m;
    int l = b._n;
    Matrix< T > res( n, m );
    for( int i = 0; i != n; i++ ) {
        for( int j = 0; j != n; j++ ) {
            for ( int k = 0; k != l; k++ )
                res[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];
        }
    }
    return res;
}

template < typename T >
Matrix< T > multiplyNaivePipe( const Matrix< T >& a, const Matrix< T >& b ) {
    assert( a._m == b._n );
    int n = a._n;
    int m = b._m;
    int l = b._n;
    Matrix< T > res( n, m );
    for( int i = 0; i != n; i++ ) {
        for( int j = 0; j != n; j++ ) {
            float x1 = 0, x2 = 0, x3 = 0, x4 = 0;
            for ( int k = 0; k != l; k += 4 ) {
                x1 += a[ i ][ k + 0 ] * b[ k + 0 ][ j ];
                x2 += a[ i ][ k + 1 ] * b[ k + 1 ][ j ];
                x3 += a[ i ][ k + 2 ] * b[ k + 2 ][ j ];
                x4 += a[ i ][ k + 3 ] * b[ k + 3 ][ j ];
            }
            res[i][j] = x1 + x2 + x3 + x4;
        }
    }
    return res;
}

template < typename T, typename Fun >
void forTiles( const Matrix< T >& m, int size, Fun f ) {
    int iCount = m._n / size;
    int jCount = m._n / size;
    for ( int i = 0; i != iCount; i++ )
        for( int j = 0; j != jCount; j++ )
            f( i * size, j * size );
}

template < typename T, int tileSize = 32 >
Matrix< T > multiplyTile( const Matrix< T >& a, const Matrix< T >& b ) {
    assert( a._m == b._n );
    int n = a._n;
    int m = b._m;
    int l = b._n;

    Matrix< T > res( n, m );
    std::vector< std::vector< T > > tile;
    std::generate_n( std::back_inserter( tile ), tileSize, [&]{ return
        std::vector< T >( tileSize ); } );
    forTiles( b, tileSize, [&]( int Bi, int Bj ) {
        for ( int x = 0; x != tileSize; x++ )
            for ( int y = 0; y != tileSize; y++ )
                tile[ x ][ y ] = b[ Bi + y ][ Bi + x ];
        for ( int Ai = 0; Ai < a._n; Ai += tileSize ) {
            int Aj = Bj;
            for ( int i = 0; i != tileSize; i++ )
                for( int j = 0; j != tileSize; j++ )
                    for ( int k = 0; k != tileSize; k++ )
                        res[ i ][ j ] += a[ Ai + i ][ Aj + k ] * tile[ j ][ k ];
        }
    } );
    return res;
}

template < typename T, int tileSize = 32 >
Matrix< T > multiplyTilePipe( const Matrix< T >& a, const Matrix< T >& b ) {
    assert( a._m == b._n );
    int n = a._n;
    int m = b._m;
    int l = b._n;

    Matrix< T > res( n, m );
    std::vector< std::vector< T > > tile;
    std::generate_n( std::back_inserter( tile ), tileSize, [&]{ return
        std::vector< T >( tileSize ); } );
    forTiles( b, tileSize, [&]( int Bi, int Bj ) {
        for ( int x = 0; x != tileSize; x++ )
            for ( int y = 0; y != tileSize; y++ )
                tile[ x ][ y ] = b[ Bi + y ][ Bi + x ];
        for ( int Ai = 0; Ai < a._n; Ai += tileSize ) {
            int Aj = Bj;
            for ( int i = 0; i != tileSize; i++ )
                for( int j = 0; j != tileSize; j++ ) {
                    T x1 = 0, x2 = 0, x3 = 0, x4 = 0;
                    for ( int k = 0; k < tileSize; k += 4 ) {
                        x1 += a[ Ai + i][ Aj + k + 0 ] + tile[ j ][ k + 0 ];
                        x2 += a[ Ai + i][ Aj + k + 1 ] + tile[ j ][ k + 1 ];
                        x3 += a[ Ai + i][ Aj + k + 2 ] + tile[ j ][ k + 2 ];
                        x4 += a[ Ai + i][ Aj + k + 3 ] + tile[ j ][ k + 3 ];
                    }
                    res[ i ][ j ] += x1 + x2 + x3 + x4;
                }
        }
    } );
    return res;
}


template < typename Fun >
void test( Fun mul ) {
    Matrix< int > A( 2, 3 );
    Matrix< int > B( 3, 2 );
    for ( int i = 0; i != 2; i++ )
        for ( int j = 0; j != 3; j++ )
            A[ i ][ j ] = i * 3 + j + 1;
    for ( int i = 0; i != 3; i++ )
        for ( int j = 0; j != 2; j++ )
            B[ i ][ j ] = i * 2 + j + 1;

    auto res = mul( A, B );
    assert( res[ 0 ][ 0 ] == 22 );
    assert( res[ 0 ][ 1 ] == 28 );
    assert( res[ 1 ][ 0 ] == 49 );
    assert( res[ 1 ][ 1 ] == 64 );
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

void validate() {
    test( multiplyNaive< int > );

    benchmark( "naive", multiplyNaive< float > );
    benchmark( "naivePipe", multiplyNaivePipe< float > );
    benchmark( "tile", multiplyTile< float > );
    benchmark( "tilePipe", multiplyTilePipe< float > );
}

using namespace ::brick::benchmark;
struct MatrixBenchmark : brick::benchmark::Group {
    MatrixBenchmark() {
        x.type = Axis::Quantitative;
        x.name = "size";
        x.normalize = Axis::Div;
        x.min = 64;
        x.max = 512;
        x.step = 2;
        x.log = true;

        y.type = Axis::Qualitative;
        y.name = "implementation";
        y.min = 0;
        y.max = 3;
        y._render = []( int i )
        {
            switch ( i )
            {
                case 0: return "Naive";
                case 1: return "Naive pipelined";
                case 2: return "Tile";
                case 3: return "Tiled pipelined";
            }
            return "Unknown";
        };
    }

    void setup( int _p, int _q ) override {
        p = _p; q = _q;

        std::mt19937 rand{ 42 };
        std::uniform_real_distribution<> dist( 1.0, 2.0 );

        const int size = 1024;
        auto gen = [&]{ return dist( rand ); };
        _a = Matrix< float >( size, size, gen );
        _b = Matrix< float >( size, size, gen );
    }

    BENCHMARK(multiply) {
        switch( q ) {
            case 0: _res = multiplyNaive( _a, _b ); break;
            case 1: _res = multiplyNaivePipe( _a, _b ); break;
            case 2: _res = multiplyTile( _a, _b ); break;
            case 3: _res = multiplyTilePipe( _a, _b ); break;
        }
    }

    Matrix< float > _a, _b, _res;
};