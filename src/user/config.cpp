#include "config.h"
#include "create_dot_obeject.h"
#include "user_default.h"
#include "player.h"
#include "title.h"
#include "scene_manager.h"
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

    auto edge = create_dot( "edge.png", 200 );
    _edg = edge;
    edge->set_position( vec2( app::getWindowSize( ) ) * vec2( 0.5 ) );
    add_child( edge );

    auto bac = create_dot_button( "back.png", 150 );
    _bac = bac;
    bac->set_position( vec2( app::getWindowSize( ) ) * vec2( 0, 1 ) + vec2( 100, -100 ) );
    add_child( bac );

    bac->on_ended = [ this ]
    {
        set_block_schedule_event( );
        change_action( [ ] { scene_manager::get_instans( )->replace( title::create( ) ); } );
    };

    auto slide = node::create( );
    _slide = slide;
    _edg.lock( )->add_child( slide );

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

    for ( int i = 0; i < _skin_names.size( ); ++i )
    {
        auto tex = renderer::sprite::create( "skin/" + _skin_names[i] + ".png" );
        tex->set_anchor_point( vec2( 0 ) );
        tex->set_position( vec2( 100 * 2, 0 ) * float( i ) );
        tex->set_content_size( vec2( 200 ) );
        slide->add_child( tex );
    }

    return true;
}
void config::update( float delta )
{
    _tap_prev_position = _tap_position;

    auto calc_select_position = _object_select_position;
    auto posx = calc_select_position.x;
    posx -= 100.0F; // 半分ずらす
    auto index = int( posx / 200.0F ); // 200毎区切りにする。
    if ( 0.0F + 70.0F < abs( fmodf( posx, 200.0F ) ) &&
         abs( fmodf( posx, 200.0F ) ) < 200.0F - 70.0F )
    {
        calc_select_position.x = index * 200.0F;
    }
    calc_select_position.x = std::min( calc_select_position.x, 0.0F );
    //calc_select_position.x = std::max( calc_select_position.x, -( _skin_names.size( ) - 1 ) * 200.0F );
    _slide.lock( )->set_position( calc_select_position );

    auto& root = user_default::get_instans( )->get_root( );
    root["select"] = _skin_names[abs( index )];
}
bool config::mouse_began( cinder::app::MouseEvent event )
{
    _tap_start_slide_object_position = _slide.lock( )->get_position( );

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
    i = 0;
    size = _bac.lock( )->get_children( ).size( );
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

