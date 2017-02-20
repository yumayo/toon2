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
    _lua = utility::lmake( "lua/title.lua", shared_from_this( ) );
    ( *_lua )["tap"] = [ this ]
    {
        get_parent( )->add_child( game::create( ) );
        remove_from_parent_user_function( [ this ] { delete _lua; } );
    };
    return true;
}
}