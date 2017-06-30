#include "title.h"
#include "create_dot_obeject.h"
#include "scene_manager.h"
#include "game.h"
#include "gacha.h"
#include "config.h"
#include "search_room.h"
#include "audio.hpp"
#include "se.h"
using namespace cinder;
namespace user
{
CREATE_CPP( title )
{
    CREATE( title );
}
bool title::init( )
{
    set_schedule_mouse_event( );

    if ( !scene_manager::get_instans( )->get_dont_destroy_node( ).lock( )->get_child_by_name( "bgm" ) )
    {
        auto bgm = ::audio::buffer_player::create( "sound/bgm.wav" );
        bgm->set_loop_begin_second( 0.0F );
        bgm->set_loop_end_second( 67.170F );
        bgm->set_name( "bgm" );
        bgm->set_loop( true );
        bgm->play( );
        scene_manager::get_instans( )->get_dont_destroy_node( ).lock( )->add_child( bgm );
    }
    add_se( "sound/start.wav" );
    add_se( "sound/gacha.wav" );
    add_se( "sound/config.wav" );
    add_se( "sound/back.wav" );
    add_se( "sound/captured.wav", 0.5F );
    add_se( "sound/slide.wav" );
    add_se( "sound/view.wav" );
    add_se( "sound/garagara.wav" );

    auto tit = create_dot( "title.png", app::getWindowWidth( ) * 0.9F );
    _title_logo = tit;
    add_child( tit );
    tit->set_position( vec2( app::getWindowSize( ) ) * vec2( 0.5F, 0.35F ) );

    auto sta = create_dot_button( "start.png", 200 );
    _start_button = sta;
    add_child( sta );
    sta->set_position( vec2( app::getWindowSize( ) ) * vec2( 0.5F, 0.75F ) );

    auto gac = create_dot_button( "gacha.png", 100 );
    _gacha_button = gac;
    add_child( gac );
    gac->set_position( vec2( app::getWindowSize( ) ) * vec2( 1.0F, 0.75F ) + vec2( -150, -60 ) );

    auto con = create_dot_button( "config.png", 100 );
    _config_button = con;
    add_child( con );
    con->set_position( vec2( app::getWindowSize( ) ) * vec2( 1.0F, 0.75F ) + vec2( -150, 60 ) );

    sta->on_ended = [ this ]
    {
        play_se( "sound/start.wav" );
        set_block_schedule_event( );
        auto search_handle = search_room::create( );
        search_handle->on_founded = [ this ]( Json::Value& root, 
                                              std::vector<feed_data> feed_buffet,
                                              std::vector<bullet_data>& bullet_buffer,
                                              std::vector<std::vector<ground_data>>& ground_buffer )
        {
            scene_manager::get_instans( )->push_back( game::create( root, feed_buffet, bullet_buffer, ground_buffer ) );
            change_action( [ ]
            {
                scene_manager::get_instans( )->pop_front( );
            } );
        };
        search_handle->on_not_found = [ this ] ( )
        {
            set_block_schedule_event( false );
        };
        add_child( search_handle );
    };
    gac->on_ended = [ this ]
    {
        play_se( "sound/gacha.wav" );
        set_block_schedule_event( );
        change_action( [ ] { scene_manager::get_instans( )->replace( gacha::create( ) ); } );
    };
    con->on_ended = [ this ]
    {
        play_se( "sound/config.wav" );
        set_block_schedule_event( );
        change_action( [ ] { scene_manager::get_instans( )->replace( config::create( ) ); } );
    };

    return true;
}
void title::change_action( std::function<void( )> end_fn )
{
    float i = 0;
    size_t size = _title_logo.lock( )->get_children( ).size( );
    for ( auto& c : _title_logo.lock( )->get_children( ) )
    {
        c->remove_all_actions( );
        c->run_action( action::sequence::create(
            action::delay::create( i ),
            action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    i = 0;
    size = _start_button.lock( )->get_children( ).size( );
    for ( auto& c : _start_button.lock( )->get_children( ) )
    {
        c->remove_all_actions( );
        c->run_action( action::sequence::create(
            action::delay::create( i ),
            action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    i = 0;
    size = _gacha_button.lock( )->get_children( ).size( );
    for ( auto& c : _gacha_button.lock( )->get_children( ) )
    {
        c->remove_all_actions( );
        c->run_action( action::sequence::create(
            action::delay::create( i ),
            action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    i = 0;
    size = _config_button.lock( )->get_children( ).size( );
    auto itr = _config_button.lock( )->get_children( ).begin( );
    for ( ; itr != --_config_button.lock( )->get_children( ).end( ); ++itr )
    {
        ( *itr )->remove_all_actions( );
        ( *itr )->run_action( action::sequence::create(
            action::delay::create( i ),
            action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    ( *itr )->remove_all_actions( );
    ( *itr )->run_action( action::sequence::create(
        action::delay::create( i ),
        action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ),
        action::call_func::create( end_fn ) ) );
    i = 0;
}
}