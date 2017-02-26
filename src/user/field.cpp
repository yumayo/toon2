#include "field.h"
#include "ground.h"
#include "feed_manager.h"
#include "player_manager_client.h"
using namespace cinder;
namespace user
{
CREATE_CPP( field )
{
    CREATE( field );
}
bool field::init( )
{
    set_name( "field" );

    auto player_manager = player_manager_client::create( );
    auto ground = ground::create( player_manager );
    auto feed_manager = feed_manager::create( player_manager, ground );

    // 1
    add_child( ground );
    // 2
    add_child( feed_manager );
    // 3
    add_child( player_manager );

    return true;
}
}

