#include "controller.h"
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

    set_schedule_touch_event( );
    set_schedule_update( );

    if ( auto s = renderer::sprite::create( "player_base.png" ) )
    {
        _base_node = s;
        add_child( s );
        s->set_visible( false );
        s->set_opacity( 0.2F );

        if ( auto s1 = renderer::sprite::create( "player_base.png" ) )
        {
            _axis_node = s1;
            s1->set_scale( vec2( 0.25 ) );
            s->add_child( s1 );

            s1->set_opacity( 0.8F );
        }
    }

    return true;
}
bool controller::touch_began( cinder::app::TouchEvent::Touch event )
{
    _base_node.lock( )->set_visible( true );

    tap_start_position = event.getPos( );

    _base_node.lock( )->set_position( tap_start_position );
    _axis_node.lock( )->set_position( axis );

    return true;
}
void controller::touch_moved( cinder::app::TouchEvent::Touch event )
{
    axis = event.getPos( ) - tap_start_position;
    float len = _base_node.lock( )->get_content_size( ).x * 0.5;
    if ( len <= length( axis ) )
        axis = normalize( axis ) * len;
    _axis_node.lock( )->set_position( axis );
}
void controller::touch_ended( cinder::app::TouchEvent::Touch event )
{
    axis = vec2( 0 );
    _base_node.lock( )->set_visible( false );
}
void controller::update( float delta )
{
    _player.lock( )->set_position( _player.lock( )->get_position( ) + axis * 0.01F );
}
cinder::vec2 controller::get_axis( )
{
    return axis;
}
}

