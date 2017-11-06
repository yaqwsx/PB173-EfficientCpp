#define BRICK_UNITTEST_MAIN
#define BRICK_UNITTEST_REG
#include <brick-unittest>
#include "JM.hpp"

template< typename T >
struct TestCharSet
{
    TEST( insert )
    {
        T x;
        x.insert( 'a' );
        x.insert( 'c' );
        x.insert( 'x' );
        ASSERT( x.count( 'a' ) );
        ASSERT( x.count( 'c' ) );
        ASSERT( x.count( 'x' ) );
        ASSERT( !x.count( 'b' ) );
        ASSERT( !x.count( 'd' ) );
        ASSERT( !x.count( 'z' ) );
    }

    TEST( intersect )
    {
        T x, y;
        x.insert( 'a' );
        x.insert( 'c' );
        x.insert( 'x' );
        y.insert( 'a' );
        y.insert( 'b' );
        y.insert( 'x' );
        T z = x & y;
        ASSERT( z.count( 'a' ) );
        ASSERT( z.count( 'x' ) );
        ASSERT( !z.count( 'b' ) );
        ASSERT( !z.count( 'c' ) );
        ASSERT( !z.count( 'd' ) );
    }

    TEST( union_ )
    {
        T x, y;
        x.insert( 'a' );
        x.insert( 'c' );
        x.insert( 'x' );
        y.insert( 'a' );
        y.insert( 'b' );
        y.insert( 'x' );
        T z = x | y;
        ASSERT( z.count( 'a' ) );
        ASSERT( z.count( 'x' ) );
        ASSERT( z.count( 'b' ) );
        ASSERT( z.count( 'c' ) );
        ASSERT( !z.count( 'd' ) );
    }
};

template struct TestCharSet< JM >;
