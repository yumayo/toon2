#include "field.h"
#include "ground.h"
#include "feed_manager.h"
#include "player_manager_client.h"
#include "player_manager_host.h"
#include "scene_manager.h"
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

    auto node = scene_manager::get_instans( )->get_dont_destroy_node( );
    auto connection = node.lock( )->get_child_by_name( "connection" );
    std::shared_ptr<player_manager> player_manager_base;
    if ( root["data"]["is_host"].asBool( ) )
    {
        player_manager_base =
            player_manager_host::create( root,
                                         std::dynamic_pointer_cast
                                         <network::udp_connection>( connection ) );
    }
    else
    {
        player_manager_base =
            player_manager_client::create( root,
                                           std::dynamic_pointer_cast
                                           <network::udp_connection>( connection ) );
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

