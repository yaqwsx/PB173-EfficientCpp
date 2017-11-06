#include <cstdint>

struct JM
{
    JM() {
        for ( int i = 0; i != 4; i++ )
            _data[ i ] = 0;
    }

    void insert( char c ) {
        int bank = c >> 6;
        int offset = c & 0b111111;
        _data[ bank ] |= 1 << offset;
    }

    void erase( char c ) {
        int bank = c >> 6;
        int offset = c & 0b111111;
        _data[ bank ] &= ~( 1 << offset );
    }

    int count( char c ) {
        int bank = c >> 6;
        int offset = c & 0b111111;
        return _data[ bank ] & ( 1 << offset );
    }

    JM operator&( const JM &o ) {
        auto r = *this;
        r._data[ 0 ] &= o._data[ 0 ];
        r._data[ 1 ] &= o._data[ 1 ];
        r._data[ 2 ] &= o._data[ 2 ];
        r._data[ 3 ] &= o._data[ 3 ];
        return r;
    }

    JM operator|( const JM &o ) {
        auto r = *this;
        r._data[ 0 ] |= o._data[ 0 ];
        r._data[ 1 ] |= o._data[ 1 ];
        r._data[ 2 ] |= o._data[ 2 ];
        r._data[ 3 ] |= o._data[ 3 ];
        return r;
    }

    uint64_t _data[ 4 ];
};
