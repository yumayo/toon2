#include "field.h"
#include "ground.h"
#include "player_manager.h"
#include "feed_manager.h"
#include "scene_manager.h"
#include "action.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( field, Json::Value& root, std::map<int, cinder::ivec2>& feeds_buffer, std::vector<std::vector<unsigned char>>& ground_buffer )
{
    CREATE( field, root, feeds_buffer, ground_buffer );
}
bool field::init( Json::Value& root, std::map<int, cinder::ivec2>& feeds_buffer, std::vector<std::vector<unsigned char>>& ground_buffer )
{
    set_name( "field" );

    auto s_player_manager = player_manager::create( root );
    auto s_ground = ground::create( s_player_manager, root, ground_buffer );
    s_player_manager->set_ground( s_ground );
    auto s_feed_manager = feed_manager::create( s_player_manager, feeds_buffer );

    // 1
    add_child( s_ground );
    // 2
    add_child( s_feed_manager );
    // 3
    add_child( s_player_manager );

    return true;
}
}

