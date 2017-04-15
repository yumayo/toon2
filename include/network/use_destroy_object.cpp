#include "use_destroy_object.h"
namespace network
{
void use_destroy_object::destroy( ) const
{
    _is_live = false;
}
bool use_destroy_object::is_destroy( ) const
{
    return !_is_live;
}
}
