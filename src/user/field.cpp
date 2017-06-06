#include "field.h"
#include "ground.h"
#include "feed_manager.h"
#include "player_manager_client.h"
#include "player_manager_host.h"
#include "scene_manager.h"
#include "action.hpp"
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
    std::shared_ptr<player_manager> s_player_manager;
    std::shared_ptr<ground> s_ground;
    std::shared_ptr<feed_manager> s_feed_manager;
    if ( root["data"]["is_host"].asBool( ) )
    {
        s_player_manager =
            player_manager_host::create( root,
                                         std::dynamic_pointer_cast
                                         <network::udp_connection>( connection ) );
        s_ground = ground::create( s_player_manager );
        s_feed_manager = feed_manager::create( s_player_manager, s_ground );
    }
    else
    {
        s_player_manager =
            player_manager_client::create( root,
                                           std::dynamic_pointer_cast
                                           <network::udp_connection>( connection ) );
        s_ground = ground::create( s_player_manager );
        s_feed_manager = feed_manager::create( s_player_manager, s_ground );
    }
    

    // 1
    add_child( s_ground );
    // 2
    add_child( s_feed_manager );
    // 3
    add_child( s_player_manager );

    return true;
}
}

