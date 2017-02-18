#include "title.h"
#include "game.h"
namespace user
{
CREATE_CPP( title )
{
    CREATE( title );
}
bool title::init( )
{
    lua = utility::lmake( "lua/title.lua", shared_from_this( ) );
    ( *lua )["tap"] = [ this ]
    {
        get_parent( )->add_child( game::create( ) );
        remove_from_parent_user_function( [ this ] { delete lua; } );
    };
    return true;
}
}