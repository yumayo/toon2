#include "field.h"
#include "ground.h"
#include "cell_manager.h"
#include "feed_manager.h"
#include "scene_manager.h"
#include "bullet_manager.h"
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

    auto cell_manager = cell_manager::create( root );
    auto bullet_manager = bullet_manager::create( cell_manager );
    auto ground = ground::create( cell_manager, bullet_manager, root, ground_buffer );
    cell_manager->set_ground( ground );
    auto feed_manager = feed_manager::create( cell_manager, feeds_buffer );

    // 1
    add_child( ground );
    // 2
    add_child( feed_manager );
    // 3
    add_child( bullet_manager );
    // 4
    add_child( cell_manager );
    

    return true;
}
}

