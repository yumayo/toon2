#include "remove_self.h"
#include "../node.h"
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
void remove_self::update( float delta )
{
    _target.lock( )->remove_from_parent( );
}

#define l_class remove_self
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( remove_self
           , l_base( action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}