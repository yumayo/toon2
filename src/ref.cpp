#include "ref.h"
#include "auto_release_pool.h"
ref::ref( )
{
}

ref::~ref( )
{
}

void ref::retain( )
{
    _reference_count += 1;
}

void ref::release( )
{
    _reference_count -= 1;
}

ref* ref::auto_release( )
{
    auto_release_pool::get_instans( )->add_child( this );
    return this;
}

unsigned int ref::get_reference_count( ) const
{
    return _reference_count;
}
