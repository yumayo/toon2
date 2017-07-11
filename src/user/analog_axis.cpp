#include "analog_axis.h"
#include <treelike/action.hpp>
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( analog_axis )
{
    CREATE( analog_axis );
}
bool analog_axis::init( )
{
    set_schedule_mouse_event( );
    set_schedule_touch_event( );

    _last_normaized_axis = normalize( vec2( 1 ) );

    _base_node = add_child( renderer::circle::create( 128.0F ) );
    _base_node->set_visible( false );
    _base_node->set_opacity( 0.2F );

    _axis_node = _base_node->add_child( renderer::circle::create( 32.0F ) );
    _axis_node->set_visible( false );
    _axis_node->set_opacity( 0.8F );

    return true;
}
cinder::vec2 analog_axis::get_normalized_axis( )
{
    float len = _base_node->get_content_size( ).x / 2;
    return _axis / len;
}
cinder::vec2 analog_axis::get_last_normalized_axis( )
{
    return _last_normaized_axis;
}
bool analog_axis::mouse_began( cinder::app::MouseEvent event )
{
    began( event.getPos( ) );
    return true;
}
void analog_axis::mouse_moved( cinder::app::MouseEvent event )
{
    moved( event.getPos( ) );
}
void analog_axis::mouse_ended( cinder::app::MouseEvent event )
{
    ended( event.getPos( ) );
}
bool analog_axis::touch_began( cinder::app::TouchEvent::Touch event )
{
    began( event.getPos( ) );
    return true;
}
void analog_axis::touch_moved( cinder::app::TouchEvent::Touch event )
{
    moved( event.getPos( ) );
}
void analog_axis::touch_ended( cinder::app::TouchEvent::Touch event )
{
    ended( event.getPos( ) );
}
void analog_axis::began( cinder::vec2 pos )
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
void analog_axis::moved( cinder::vec2 pos )
{
    _axis = pos - _tap_start_position;
    float len = _base_node->get_content_size( ).x * 0.5;
    if ( len <= length( _axis ) )
        _axis = normalize( _axis ) * len;
    _axis_node->set_position( _axis );
    _last_normaized_axis = normalize( _axis );
}
void analog_axis::ended( cinder::vec2 pos )
{
    _axis = vec2( 0 );
    _axis_node->run_action( action::sequence::create( action::ease<EaseOutExpo>::create( action::move_to::create( 0.2F, _axis ) ),
                                                      action::call_func::create( [ this ] { _base_node->run_action( action::fade_out::create( 0.2F ) ); } ),
                                                      action::fade_out::create( 0.2F ),
                                                      action::call_func::create( [ this ] { _base_node->set_visible( false ); _axis_node->set_visible( false ); } ) ) );
}
}