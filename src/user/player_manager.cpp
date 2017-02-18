#include "player_manager.h"
#include "player.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager )
{
    CREATE( player_manager );
}
bool player_manager::init( )
{
    set_name( "player_manager" );

    auto own = player::create( );
    own->set_name( "own" );
    add_child( own );

    return true;
}
}

