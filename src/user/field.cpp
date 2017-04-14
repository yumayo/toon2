#include "field.h"
#include "ground.h"
#include "feed_manager.h"
#include "player_manager_client.h"
#include "player_manager_host.h"
using namespace cinder;
namespace user
{
CREATE_CPP( field, Json::Value& root )
{
    CREATE( field, root );
}
bool field::init( Json::Value& root )
{
    set_name( "field" );

    std::shared_ptr<player_manager> player_manager_base;
    if ( root["data"]["is_host"].asBool( ) )
    {
        player_manager_base = player_manager_host::create( root );
    }
    else
    {
        player_manager_base = player_manager_client::create( root );
    }
    auto ground = ground::create( player_manager_base );
    auto feed_manager = feed_manager::create( player_manager_base, ground );

    // 1
    add_child( ground );
    // 2
    add_child( feed_manager );
    // 3
    add_child( player_manager_base );

    return true;
}
}

