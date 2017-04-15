#include "remove_self.h"
#include "node.h"
namespace action
{
CREATE_CPP( remove_self )
{
    CREATE( remove_self );
}
bool remove_self::init( )
{
    return true;
}
float remove_self::update( float delta )
{
    _target.lock( )->remove_from_parent( );
    return delta;
}
}