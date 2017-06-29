#include "field.h"
#include "ground.h"
#include "cell_manager.h"
#include "feed_manager.h"
#include "scene_manager.h"
#include "bullet_manager.h"
#include "action.hpp"
#include "cinder/Camera.h"
#include "user_default.h"
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
    cell_manager->set_bullet_manager( bullet_manager );
    auto feed_manager = feed_manager::create( cell_manager, feeds_buffer );

    // 1
    add_child( ground );
    // 2
    add_child( feed_manager );
    // 3
    add_child( bullet_manager );
    // 4
    add_child( cell_manager );

    _player = cell_manager->get_player( );

    return true;
}
void field::render( )
{
    auto player = std::dynamic_pointer_cast<user::player>( _player.lock( ) );

    auto radius = player->get_radius( ) - player->get_startup_radius( );

    auto min_radius = user_default::get_instans( )->get_root( )["system"]["min_radius"].asFloat( );
    auto max_radius = user_default::get_instans( )->get_root( )["system"]["max_radius"].asFloat( );

    radius = glm::clamp( radius,
                         min_radius,
                         max_radius );

    float r = radius / ( max_radius - min_radius );

    CameraPersp camera;
    camera.setPerspective( 75.0F, app::getWindowAspectRatio( ), 1.0F, 1100.0F );
    camera.lookAt( vec3( player->get_position( ), -1000 * easeOutCirc( r ) ), vec3( player->get_position( ), 0 ) );
    camera.setWorldUp( vec3( 0, -1, 0 ) );
    gl::setMatrices( camera );
}
}

