#pragma once

#include <forward_list>
#include <vector>
#include <algorithm>
#include <experimental/optional>
#include <random>
#include <limits>

int randomInt() {
    static std::random_device r;
    static std::default_random_engine e1( r() );
    static std::uniform_int_distribution< int > uniform_dist(
        std::numeric_limits< int >::min(),
        std::numeric_limits< int >::max()
    );
    return uniform_dist( e1 );
}

template < typename T >
struct BucketSet {
    BucketSet( double thresholdLoad = 0.75 )
        : _size( 0 ), _threshold( thresholdLoad )
    {
        _data.resize( 32 );
    }

    void insert( const T& item ) {
        if ( static_cast< double >( _size ) / _data.size() > _threshold )
            _resize();
        auto& bucket = _getBucket( item );
        auto r = std::find( bucket.begin(), bucket.end(), item );
        if ( r != bucket.end() )
            return;
        bucket.push_front( item );
        _size++;
    }

    T *find( const T& item ) {
        auto& bucket = _getBucket( item );
        auto r = std::find( bucket.begin(), bucket.end(), item );
        return r == bucket.end() ? nullptr : &( *r );
    }

    std::forward_list< T >& _getBucket( const T& t ) {
        return _data[ std::hash< T >()( t ) % _data.size() ];
    }

    void _resize() {
        _size = 0;
        std::vector< std::forward_list< T > > data( _data.size() * 2 );
        std::swap( _data, data );
        for ( const auto& bucket : data )
            for ( auto&& item : bucket )
                insert( item );
    }

    std::vector< std::forward_list< T > > _data;
    int _size;
    double _threshold;
};

template < typename T >
struct LinearSet {
    LinearSet( double thresholdLoad = 0.75 )
        : _size( 0 ), _threshold( thresholdLoad )
    {
        _data.resize( 32 );
    }

    void insert( const T& t ) {
        if ( static_cast< double >( _size ) / _data.size() > _threshold )
            _resize();
        auto& bucket = _find( t );
        if ( !bucket )
            _size++;
        bucket = t;
    }

    T *find( const T& t ) {
        auto bucket = _find( t );
        return bucket ? &bucket.value() : nullptr;
    }

    std::experimental::optional< T >& _find( const T& t ) {
        auto item = _data.begin() + ( std::hash< T >()( t ) % _data.size() );
        while ( *item && item->value() != t ) {
            item++;
            if ( item == _data.end() )
                item = _data.begin();
        }
        return *item;
    }

    void _resize() {
        _size = 0;
        std::vector< std::experimental::optional< T > > data( _data.size() * 2 );
        std::swap( _data, data );
        for ( const auto& item : data )
            if ( item )
                insert( *item );
    }

    std::vector< std::experimental::optional< T > > _data;
    int _size;
    double _threshold;
};