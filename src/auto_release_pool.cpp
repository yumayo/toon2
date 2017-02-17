#include "auto_release_pool.h"
#include "boost/range/algorithm/remove_if.hpp"

auto_release_pool* auto_release_pool::_instans = nullptr;

auto_release_pool * auto_release_pool::get_instans( )
{
    if ( !_instans )
    {
        _instans = new auto_release_pool( );
    }
    return _instans;
}

void auto_release_pool::remove_instans( )
{
    delete _instans;
}

void auto_release_pool::add_child( ref* object )
{
    _objects.emplace_back( object );
}

void auto_release_pool::collect_garbage( )
{
    if ( !_objects.empty( ) )
    {
        auto dies = boost::remove_if( _objects, [ ] ( ref* obj )
        {
            return obj->get_reference_count( ) == 0;
        } );
        for ( auto itr = dies; itr != _objects.end( ); ++itr )
        {
            delete *itr;
            *itr = nullptr;
        }
        _objects.erase( dies );
    }
}

auto_release_pool::auto_release_pool( )
{
}
