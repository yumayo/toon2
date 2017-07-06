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
    _last_normaized_axis = normalize( vec2( 1 ) );

    _player = player;
    _ground = ground;
    _bullet_manager = bullet_manager;

    set_schedule_mouse_event( );
    set_schedule_touch_event( );
    set_schedule_key_event( );
    set_schedule_update( );

    _base_node = add_child( renderer::circle::create( 128.0F ) );
    _base_node->set_visible( false );
    _base_node->set_opacity( 0.2F );

    _axis_node = _base_node->add_child( renderer::circle::create( 32.0F ) );
    _axis_node->set_visible( false );
    _axis_node->set_opacity( 0.8F );

    on_key_event( app::KeyEvent::KEY_SPACE, [ this ] ( cinder::app::KeyEvent e )
    {
        Json::Value data;
        if ( _player->parse_bullet_data( &data, _last_normaized_axis ) )
        {
            _bullet_manager->create_bullet( data );
        }
    } );
    on_key_event( app::KeyEvent::KEY_UP, [ this ] ( cinder::app::KeyEvent e )
    {
        _player->capture( 10.0F );
    } );
    on_key_event( app::KeyEvent::KEY_DOWN, [ this ] ( cinder::app::KeyEvent e )
    {
        _player->capture( -10.0F );
    } );

    return true;
}
bool controller::mouse_began( cinder::app::MouseEvent event )
{
    began( event.getPos( ) );
    return true;
}
void controller::mouse_moved( cinder::app::MouseEvent event )
{
    moved( event.getPos( ) );
}
void controller::mouse_ended( cinder::app::MouseEvent event )
{
    ended( event.getPos( ) );
}
bool controller::touch_began( cinder::app::TouchEvent::Touch event )
{
    began( event.getPos( ) );
    return true;
}
void controller::touch_moved( cinder::app::TouchEvent::Touch event )
{
    moved( event.getPos( ) );
}
void controller::touch_ended( cinder::app::TouchEvent::Touch event )
{
    ended( event.getPos( ) );
}
void controller::key_down( cinder::app::KeyEvent e )
{
    auto itr = _key_events.find( e.getCode( ) );
    if ( itr != _key_events.end( ) )
    {
        if ( itr->second ) itr->second( e );
    }
}
void controller::update( float delta )
{
    if ( _player )
    {
        auto system_speed = user_default::get_instans( )->get_root( )["system"]["speed"].asFloat( );
        cinder::vec2 speed = get_normalized_axis( ) * (float)system_speed * delta;
        auto system_max_radius = user_default::get_instans( )->get_root( )["system"]["max_radius"].asFloat( );
        speed *= ( 1.0F - easeOutSine( _player->get_radius( ) / system_max_radius ) * 0.5F );
        _player->set_position( _player->get_position( ) + speed );
        _ground->collide( _player );
    }
}
cinder::vec2 controller::get_normalized_axis( )
{
    float len = _base_node->get_content_size( ).x * 0.5F;
    return _axis / len;
}
void controller::began( cinder::vec2 pos )
{
    if ( _base_node->is_running_action( ) )
    {
        _base_node->remove_all_actions( );
    }
    if ( _axis_node->is_running_action( ) )
    {
        _axis_node->remove_all_actions( );
    }

    _base_node->set_visible( true );
    _base_node->set_opacity( 0.2F );
    _axis_node->set_visible( true );
    _axis_node->set_opacity( 0.8F );

    _tap_start_position = pos;

    _base_node->set_position( _tap_start_position );
    _axis_node->set_position( _axis );
    _last_normaized_axis = normalize( _axis );
}
void controller::moved( cinder::vec2 pos )
{
    _axis = pos - _tap_start_position;
    float len = _base_node->get_content_size( ).x * 0.5;
    if ( len <= length( _axis ) )
        _axis = normalize( _axis ) * len;
    _axis_node->set_position( _axis );
    _last_normaized_axis = normalize( _axis );
}
void controller::ended( cinder::vec2 pos )
{
    _axis = vec2( 0 );

    auto controller_fade_out = [ this ]
    {
        _base_node->run_action( action::fade_out::create( 0.2F ) );
    };
    _axis_node->run_action( action::sequence::create( action::ease<EaseOutExpo>::create( action::move_to::create( 0.2F, _axis ) ),
                                                              action::call_func::create( std::move( controller_fade_out ) ),
                                                              action::fade_out::create( 0.2F ),
                                                              action::call_func::create( [ this ] { _base_node->set_visible( false ); _axis_node->set_visible( false ); } ) ) );
}
void controller::on_key_event( int code, std::function<void( cinder::app::KeyEvent )> func )
{
    _key_events.insert( std::make_pair( code, func ) );
}
}

