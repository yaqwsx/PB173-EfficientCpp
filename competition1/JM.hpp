#include <cstdint>
#include <array>

struct JM
{
    JM() : _data( { 0, 0, 0, 0 } ){}

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
        for ( int i = 0; i != 4; i++ )
            r._data[ i ] &= o._data[ i ];
        return r;
    }

    JM operator|( const JM &o ) {
        auto r = *this;
        for ( int i = 0; i != 4; i++ )
            r._data[ i ] |= o._data[ i ];
        return r;
    }

    std::array< uint64_t, 4 > _data;
};
