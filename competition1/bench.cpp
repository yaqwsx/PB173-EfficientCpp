#define BRICK_BENCHMARK_MAIN
#define BRICK_BENCHMARK_REG
#include <brick-benchmark>
#include "JM.hpp"

using namespace brick::benchmark;

template< typename T >
struct BenchCharSet : Group
{
    BenchCharSet()
    {
        x.type = Axis::Disabled;
        y.type = Axis::Benchmarks;
    }

    std::string describe()
    {
        return "category:charset type:" + brick::unittest::_typeid< T >();
    }

    BENCHMARK( insert )
    {
        T x;
        for ( int i = 0; i < 1000; ++i )
            x.insert( i % 12 + 'a' );
        for ( int i = 0; i < 1000; ++i )
            x.count( i % 12 + 'a' );
    }

    BENCHMARK( intersect )
    {
        T x, y, z;
        for ( int i = 0; i < 1000; ++i )
        {
            x.insert( i % 4 + 'd' );
            y.insert( i % 4 + 'b' );
            z = x & y;
        }
    }

    BENCHMARK( union_ )
    {
        T x, y, z;
        for ( int i = 0; i < 1000; ++i )
        {
            x.insert( i % 4 + 'd' );
            y.insert( i % 4 + 'b' );
            z = x | y;
        }
    }

    BENCHMARK( sum )
    {
        insert();
        intersect();
        union_();
    }
};

template struct BenchCharSet< JM >;
