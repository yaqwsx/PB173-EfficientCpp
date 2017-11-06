#include <cstdint>
#include <immintrin.h>


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
        *reinterpret_cast< __m256d* >( &r._data )
            = _mm256_and_pd( *reinterpret_cast< __m256d* >( &r._data ),
                             *reinterpret_cast< const __m256d* >( &o._data ) );
        return r;
    }

    JM operator|( const JM &o ) {
        auto r = *this;
        *reinterpret_cast< __m256d* >( &r._data )
            = _mm256_or_pd( *reinterpret_cast< __m256d* >( &r._data ),
                         *reinterpret_cast< const __m256d* >( &o._data ) );
        return r;
    }

    uint64_t _data[ 4 ];
};
