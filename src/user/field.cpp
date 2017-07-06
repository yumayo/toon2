#include "field.h"
#include "ground.h"
#include "cell_manager.h"
#include "feed_manager.h"
#include <treelike/scene_manager.h>
#include "bullet_manager.h"
#include <treelike/action.hpp>
#include "cinder/Camera.h"
#include <treelike/user_default.h>
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( field,
            Json::Value& root,
            std::vector<feed_data> feed_buffet,
            Json::Value& bullet_buffer,
            std::vector<std::vector<ground_data>>& ground_buffer )
{
    CREATE( field, root, feed_buffet, bullet_buffer, ground_buffer );
}
bool field::init( Json::Value& root,
                  std::vector<feed_data> feed_buffet,
                  Json::Value& bullet_buffer,
                  std::vector<std::vector<ground_data>>& ground_buffer )
{
    set_name( "field" );

    auto cell_manager = cell_manager::create( root );
    auto bullet_manager = bullet_manager::create( cell_manager, bullet_buffer );
    auto ground = ground::create( cell_manager, bullet_manager, root, ground_buffer );
    cell_manager->set_ground( ground );
    cell_manager->set_bullet_manager( bullet_manager );
    auto feed_manager = feed_manager::create( cell_manager, feed_buffet );

    // 1
    _ground = add_child( ground );
    // 2
    _feed_manager = add_child( feed_manager );
    // 3
    _bullet_manager = add_child( bullet_manager );
    // 4
    _cell_manager = add_child( cell_manager );

    _player = cell_manager->get_player( );

    return true;
}
}

