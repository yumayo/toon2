#include "button_bullet_firing.h"
#include "analog_axis.h"
#include "spike.h"
#include "skin.h"
#include "parser.h"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( button_bullet_firing_function )
{
    CREATE( button_bullet_firing_function );
}
bool button_bullet_firing_function::init( )
{
    circle_button_opacity_action::init( );
    set_schedule_key_event( );
    on_key_event( app::KeyEvent::KEY_SPACE, [ this ] ( cinder::app::KeyEvent e )
    {
        auto m = get_world_matrix( );
        auto pos = vec2( m[2][0], m[2][1] );
        began( pos );
        ended( pos );
    } );
    return true;
}
void button_bullet_firing_function::key_down( cinder::app::KeyEvent e )
{
    auto itr = _key_events.find( e.getCode( ) );
    if ( itr != _key_events.end( ) )
    {
        if ( itr->second ) itr->second( e );
    }
}
void button_bullet_firing_function::on_key_event( int code, std::function<void( cinder::app::KeyEvent )> func )
{
    _key_events.insert( std::make_pair( code, func ) );
}
CREATE_CPP( button_bullet_firing, softptr<player> player, softptr<bullet_manager> bullet_manager )
{
    CREATE( button_bullet_firing, player, bullet_manager );
}
bool button_bullet_firing::init( softptr<player> player, softptr<bullet_manager> bullet_manager )
{
    set_schedule_update( );

    _player = player;
    _bullet_manager = bullet_manager;

    float radius = 80.0F;

    _base = add_child( user::spike::create( radius, radius, 10.0F ) );
    _mask = _base->add_child( _player->get_skin_relative_path( ).empty( ) ?
                              renderer::circle::create( radius ) :
                              skin::create( radius, radius, _player->get_skin_relative_path( ) ) );
    if ( _player->get_skin_relative_path( ).empty( ) )
    {
        _mask->set_color( _player->get_color( ) );
    }
    _mask->set_scale( vec2( 0.9F ) );
    auto button_cover = add_child( treelike::renderer::circle::create( radius ) );
    button_cover->set_scale( vec2( 1.2F ) );
    button_cover->set_opacity( 0.5F );

    _circle_button = add_child( button_bullet_firing_function::create( ) );
    _circle_button->set_content_size( vec2( radius * 2 ) );
    _circle_button->on_ended = [ this ]
    {
        Json::Value data;
        if ( parser::bullet_fireworks( &data, _player, get_parent( )->get_child_by_name( "analog_axis" ) ) )
        {
            _bullet_manager->create_bullet( data, 2 );
        }
    };

    _base->set_opacity( _circle_button->get_opacity( ) );

    set_position( app::getWindowSize( ) - ivec2( 200, 150 ) );

    return true;
}
void button_bullet_firing::update( float delta )
{
    _base->set_opacity( _circle_button->get_opacity( ) );
}
}
