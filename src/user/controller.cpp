#include "controller.h"
#include <treelike/renderer/rect_edge.h>
#include <treelike/action.hpp>
#include <treelike/scene_manager.h>
#include "title.h"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( controller, softptr<player> player, softptr<ground> ground, softptr<bullet_manager> bullet_manager )
{
    CREATE( controller, player, ground, bullet_manager );
}
bool controller::init( softptr<player> player, softptr<ground> ground, softptr<bullet_manager> bullet_manager )
{
    set_schedule_key_event( );
    set_schedule_update( );

    _player = player;
    _ground = ground;
    _bullet_manager = bullet_manager;

    _analog_axis = add_child( analog_axis::create( ) );
    _debug_command = add_child( debug_command::create( player, bullet_manager ) );
    _button_bullet_firing = add_child( button_bullet_firing::create( player, bullet_manager ) );

    return true;
}
void controller::update( float delta )
{
    if ( _player )
    {
        auto system_speed = user_default::get_instans( )->get_root( )["system"]["speed"].asFloat( );
        cinder::vec2 speed = _analog_axis->get_normalized_axis( ) * (float)system_speed * delta;
        auto system_max_radius = user_default::get_instans( )->get_root( )["system"]["max_radius"].asFloat( );
        speed *= ( 1.0F - easeOutSine( _player->get_radius( ) / system_max_radius ) * 0.5F );
        _player->set_position( _player->get_position( ) + speed );
        _ground->collide( _player );
    }
}
}

