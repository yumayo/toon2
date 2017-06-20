#include "config.h"
#include "create_dot_obeject.h"
#include "user_default.h"
#include "player.h"
#include "title.h"
#include "scene_manager.h"
#include "se.h"
using namespace cinder;
using namespace action;
namespace user
{
CREATE_CPP( config )
{
    CREATE( config );
}
bool config::init( )
{
    set_schedule_update( );
    set_schedule_mouse_event( );
    set_schedule_touch_event( );

    auto slide_manager = node::create( );
    slide_manager->set_position( vec2( app::getWindowSize( ) ) * vec2( 0.5F ) );
    slide_manager->set_pivot( vec2( 0.5F ) );
    add_child( slide_manager );

    auto slider = node::create( );
    // デフォルト仕様。
    _skin_names.emplace_back( "null" );
    auto& root = user_default::get_instans( )->get_root( );
    auto& skin = root["skin"];
    for ( auto& name : skin.getMemberNames( ) )
    {
        if ( skin[name].asBool( ) )
        {
            _skin_names.emplace_back( name );
        }
    }

    std::string user_select_skin_name = user_default::get_instans( )->get_root( )["select_skin_name"].asString( );
    int select_index = 0;
    for ( int i = 0; i < _skin_names.size( ); ++i )
    {
        if ( _skin_names[i] == user_select_skin_name ) select_index = i;
        auto tex = renderer::sprite::create( "skin/" + _skin_names[i] + ".png" );
        tex->set_anchor_point( vec2( 0.5F ) );
        tex->set_content_size( vec2( _skin_width ) );
        tex->set_position( vec2( _skin_width, 0 ) * float( i ) );
        slider->add_child( tex );
    }
    _slider = slider;
    // スライド方向がマイナスなので * -1 しています。
    _object_select_position = vec2( select_index * -1 * _skin_width, 0 );
    _slider.lock( )->set_position( _object_select_position );
    slide_manager->add_child( slider );

    auto edge = create_dot( "edge.png", _skin_width );
    _edg = edge;
    slide_manager->add_child( edge );

    auto bac = create_dot_button( "back.png", 150.0F );
    _bac = bac;
    bac->set_position( vec2( app::getWindowSize( ) ) * vec2( 0, 1 ) + vec2( 100, -100 ) );
    add_child( bac );

    bac->on_ended = [ this ]
    {
        play_se( "sound/back.wav" );
        set_block_schedule_event( );
        change_action( [ ] { scene_manager::get_instans( )->replace( title::create( ) ); } );
    };

    return true;
}
void config::update( float delta )
{
    _tap_prev_position = _tap_position;

    auto calc_select_position = _object_select_position;
    auto& posx = calc_select_position.x;
    posx = glm::clamp( posx, ( _skin_names.size( ) - 1 ) * -_skin_width, 0.0F );
    auto index = int( ( posx - 35.0F ) / _skin_width ); // 200毎区切りにする。
    auto single_select_skin_posx = abs( fmodf( posx, _skin_width ) );
    bool prev_is_switched = _is_switched;
    _is_switched = single_select_skin_posx < 35.0F || _skin_width - 35.0F < single_select_skin_posx;
    if ( _is_switched )
    {
        posx = index * _skin_width;
    }
    if ( !prev_is_switched && _is_switched )
    {
        play_se( "sound/slide.wav" );
    }
    _slider.lock( )->set_position( calc_select_position );

    user_default::get_instans( )->get_root( )["select_skin_name"] = _skin_names[abs( index )] == "null" ? Json::Value::null : _skin_names[abs( index )];
}
bool config::mouse_began( cinder::app::MouseEvent event )
{
    _tap_start_slide_object_position = _slider.lock( )->get_position( );

    _tap_start_position = event.getPos( );
    _tap_position = event.getPos( );

    return true;
}
void config::mouse_moved( cinder::app::MouseEvent event )
{
    _tap_position = event.getPos( );

    auto slide_size = _tap_position.x - _tap_start_position.x;
    _object_select_position = _tap_start_slide_object_position + vec2( slide_size, 0 );
}
void config::mouse_ended( cinder::app::MouseEvent event )
{
}
bool config::touch_began( cinder::app::TouchEvent::Touch event )
{
    return true;
}
void config::touch_moved( cinder::app::TouchEvent::Touch event )
{
}
void config::touch_ended( cinder::app::TouchEvent::Touch event )
{
}
void user::config::change_action( std::function<void( )> end_fn )
{
    {
        float i = 0;
        int size = _slider.lock( )->get_children( ).size( );
        for ( auto& c : _slider.lock( )->get_children( ) )
        {
            c->remove_all_actions( );
            c->run_action( sequence::create(
                delay::create( i ),
                ease<EaseOutBounce>::create( scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
            i += 1.0F / size;
        }
    }
    {
        float i = 0;
        int size = _edg.lock( )->get_children( ).size( );
        for ( auto& c : _edg.lock( )->get_children( ) )
        {
            c->remove_all_actions( );
            c->run_action( sequence::create(
                delay::create( i ),
                ease<EaseOutBounce>::create( scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
            i += 1.0F / size;
        }
    }
    {
        float i = 0;
        int size = _bac.lock( )->get_children( ).size( );
        auto itr = _bac.lock( )->get_children( ).begin( );
        for ( ; itr != --_bac.lock( )->get_children( ).end( ); ++itr )
        {
            ( *itr )->remove_all_actions( );
            ( *itr )->run_action( sequence::create(
                delay::create( i ),
                ease<EaseOutBounce>::create( scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
            i += 1.0F / size;
        }
        ( *itr )->remove_all_actions( );
        ( *itr )->run_action( sequence::create(
            delay::create( i ),
            ease<EaseOutBounce>::create( scale_to::create( 0.2F, vec2( 0 ) ) ),
            call_func::create( end_fn ) ) );
    }
}
}

