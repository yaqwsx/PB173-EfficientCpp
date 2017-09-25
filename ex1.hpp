#pragma once

#include <vector>
#include <set>
#include <cassert>

template < typename T, int limit >
struct BoundedVectorBuffer {
    BoundedVectorBuffer() {
        _data.reserve( limit );
    }

    void insert( const T& t ) {
        auto pos = std::lower_bound( _data.begin(), _data.end(), t );
        if ( pos != _data.end() && *pos == t )
            return;
        if ( _data.size() == limit )
            _data.erase( _data.begin() );
        pos = std::lower_bound( _data.begin(), _data.end(), t );
        _data.insert( pos, t );
    }

    const T& top() {
        assert( !_data.empty() );
        return _data.back();
    }

    void remove() {
        assert( !_data.empty() );
        _data.pop_back();
    }

    std::vector< T > _data;
};

template < typename T, int limit >
struct BoundedSetBuffer {
    void insert( const T& t ) {
        auto elem = _data.find( t );
        if ( elem != _data.end() )
            return;
        if ( _data.size() == limit )
            _data.erase( _data.begin() );
        _data.insert( t );
    }

    const T& top() {
        assert( !_data.empty() );
        return *_data.rbegin();
    }

    void remove() {
        assert( !_data.empty() );
        _data.erase( --_data.end() );
    }

    std::set< T > _data;
};

template < template< typename, int > class Buffer >
void testBuffer() {
    Buffer< int, 10 > buff;
    for ( int i = 9; i >= 0; i-- )
        buff.insert( i );
    buff.insert( 5 );
    for ( int i = 9; i >= 0; i-- ) {
        assert( buff.top() == i );
        buff.remove();
    }

    for ( int i = 0; i != 10; i++ )
        buff.insert( i );
    buff.insert( 5 );
    for ( int i = 9; i >= 0; i-- ) {
        assert( buff.top() == i );
        buff.remove();
    }
}