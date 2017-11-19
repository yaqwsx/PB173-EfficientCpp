#include <bitset>
#include <cstdint>
#include <limits>
#include <array>
#include <memory>
#include <cassert>

#define BRICK_BENCHMARK_REG
#define BRICK_BENCHMARK_MAIN
#include <brick-benchmark>


struct Set16A {
    void insert( uint16_t n ) { _data[ n ] = true; }
    void erase( uint16_t n ) { _data[ n ] = false; }
    int count( uint16_t n ) { return _data[ n ]; }
    Set16A operator&( const Set16A& o ) {
        Set16A x = *this;
        x._data &= o._data;
        return x;
    }
    Set16A operator|( const Set16A& o ) {
        Set16A x = *this;
        x._data |= o._data;
        return x;
    }
    std::bitset< std::numeric_limits< uint16_t >::max() > _data;
};

template < int Depth >
struct Node {
    Node() = default;
    Node( const Node& other ) {
        for ( int i = 0; i != _data.size(); i++ ) {
            if ( other._data[ i ] )
                _data[ i ].reset( new Node< Depth - 1 >( *other._data[ i ] ) );
        }
    }

    Node& operator=( Node o ) {
        std::swap( _data, o._data );
        return *this;
    }

    bool find( int i ) {
        int idx = i & 0x0f;
        if ( !_data[ idx ] )
            return false;
        return _data[ idx ]->find( i >> 4 );
    }

    void insert( int i, bool val ) {
        int idx = i & 0x0f;
        if ( !_data[ idx ] )
            _data[ idx ].reset( new Node< Depth - 1 >() );
        return _data[ idx ]->insert( i >> 4, val );
    }

    Node operator|=( const Node& other ) {
        for ( int i = 0; i != _data.size(); i++ ) {
            if ( _data[ i ] && other._data[ i ] )
                *_data[ i ] |= *other._data[ i ];
            else if ( other._data[ i ] )
                _data[ i ].reset( new Node< Depth - 1 >( *other._data[ i ] ) );
        }
    }
    std::array< std::unique_ptr< Node< Depth - 1 > >, 16 > _data;
};

template <>
struct Node< 0 > : std::bitset< 16 > {
    void insert( int i, bool val ) {
        ( *this )[ i ] = val;
    }

    bool find( int i ) {
        return ( *this )[ i ];
    }
};

struct Set16B {
    void insert( uint16_t n ) {
        _root.insert( n, true );
    }
    void erase( uint16_t n ) {
        _root.insert( n, false );
    }
    int count( uint16_t n ) {
        return _root.find( n ) ? 1 : 0;
    }
    Set16B operator|( const Set16B& other ) {
        // Yaiks - this crashes: I call the destructor twice. But why? Why?
        Set16B ret = *this;
        ret._root |= other._root;
        return ret;
    }

    Node< 3 > _root;
};

template < typename Set >
void validate() {
    Set a;
    a.insert( 0 );
    assert( a.count( 0 ) );
    for ( int i = 0; i != 15; i++ ) {
        a.insert( 1 << i );
        assert( a.count( 1 << i ) );
    }

    Set b;
    for ( int i = 0; i != 1024; i++ )
        b.insert( i );

    Set c = a | b;
    for( int i = 0; i != 1024; i++ )
        assert( c.count( i ) );
    for ( int i = 0; i != 15; i++ ) {
        assert( c.count( 1 << i ) );
    }

    Set d = b | a;
    for( int i = 0; i != 1024; i++ )
        assert( d.count( i ) );
    for ( int i = 0; i != 15; i++ ) {
        assert( d.count( 1 << i ) );
    }
}


using namespace ::brick::benchmark;
struct SetBenchmark : brick::benchmark::Group {
    SetBenchmark() {
        x.type = Axis::Quantitative;
        x.name = "size";
        x.normalize = Axis::Div;
        x.min = 64;
        x.max = 1024*1024;
        x.step = 2;
        x.log = true;

        y.type = Axis::Qualitative;
        y.name = "implementation";
        y.min = 0;
        y.max = 1;
        y._render = []( int i )
        {
            switch ( i )
            {
                case 0: return "bitset";
                case 1: return "trie";
            }
            return "Unknown";
        };
    }

    template < typename Set >
    void benchmark() {
        Set a;
        std::mt19937 rand{ 42 };
        std::uniform_int_distribution<> dist( 0, std::numeric_limits< uint16_t >::max() );
        std::uniform_int_distribution<> act( 0, 1 );
        for ( int i = 0; i != p; i++ ) {
            if ( act( rand ) )
                a.insert( dist( rand ) );
            else
                a.erase( dist( rand ) );
        }
    }

    void setup( int _p, int _q ) override {
        p = _p; q = _q;
    }

    BENCHMARK(set) {
        switch( q ) {
            case 0: benchmark< Set16A >(); break;
            case 1: benchmark< Set16B >(); break;
        }
    }
};