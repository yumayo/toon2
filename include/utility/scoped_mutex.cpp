#include "scoped_mutex.h"
namespace utility
{
scoped_mutex::scoped_mutex( recursion_usable_mutex & mutex )
    : _mutex( mutex )
{
    auto& itr = _mutex.emplace( );
    if ( itr.second )
    {
        _mutex.lock( );
    }
    else
    {
        ++itr.first->second;
    }
}
scoped_mutex::~scoped_mutex( )
{
    auto& duplication_count = _mutex.get_duplication_count( );
    if ( duplication_count == 0 )
    {
        _mutex.unlock( );
        _mutex.erase( );
    }
    else
    {
        --duplication_count;
    }
}
}
