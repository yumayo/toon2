#include "controller.h"
#include "../renderer/rect_edge.h"
#include "../action/action.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( controller, std::weak_ptr<node> player )
{
    CREATE( controller, player );
}
bool controller::init( std::weak_ptr<node> player )
{
    _player = player;

    set_schedule_mouse_event( );
    set_schedule_touch_event( );
    set_schedule_update( );

    if ( auto s = renderer::circle::create( 128.0F ) )
    {
        _base_node = s;
        add_child( s );
        s->set_visible( false );
        s->set_opacity( 0.2F );

        if ( auto s1 = renderer::circle::create( 32.0F ) )
        {
            _axis_node = s1;
            s->add_child( s1 );

            s1->set_opacity( 0.8F );
        }
    }

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
void controller::update( float delta )
{
    if ( _player.lock( ) )
        _player.lock( )->set_position( _player.lock( )->get_position( ) + _axis * delta );
}
cinder::vec2 controller::get_axis( )
{
    return _axis;
}
void controller::began( cinder::vec2 pos )
{
    if ( _base_node.lock( )->is_running_action( ) )
    {
        _base_node.lock( )->remove_all_actions( );
    }
    if ( _axis_node.lock( )->is_running_action( ) )
    {
        _axis_node.lock( )->remove_all_actions( );
    }

    _base_node.lock( )->set_visible( true );
    _base_node.lock( )->set_opacity( 0.2F );
    _axis_node.lock( )->set_opacity( 0.8F );

    _tap_start_position = pos;

    _base_node.lock( )->set_position( _tap_start_position );
    _axis_node.lock( )->set_position( _axis );
}
void controller::moved( cinder::vec2 pos )
{
    _axis = pos - _tap_start_position;
    float len = _base_node.lock( )->get_content_size( ).x * 0.5;
    if ( len <= length( _axis ) )
        _axis = normalize( _axis ) * len;
    _axis_node.lock( )->set_position( _axis );
}
void controller::ended( cinder::vec2 pos )
{
    _axis = vec2( 0 );

    auto controller_fade_out = [ this ]
    {
        _base_node.lock( )->run_action( action::fade_out::create( 0.2F ) );
    };
    _axis_node.lock( )->run_action( action::sequence::create( action::ease<EaseOutExpo>::create( action::move_to::create( 0.2F, _axis ) ),
                                                              action::call_func::create( std::move( controller_fade_out ) ),
                                                              action::fade_out::create( 0.2F ),
                                                              action::call_func::create( [ this ] { _base_node.lock( )->set_visible( false ); } ) ) );
}
}

