#include "title.h"
#include "create_dot_obeject.h"
#include "scene_manager.h"
#include "game.h"
#include "gacha.h"
#include "config.h"
#include "search_room.h"
#include "audio.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( title )
{
    CREATE( title );
}
bool title::init( )
{
    auto dont_deatroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    if ( !dont_deatroy_node.lock( )->get_child_by_name( "bgm" ) )
    {
        auto bgm = ::audio::buffer_player::create( "sound/bgm.wav" );
        bgm->set_loop_begin_second( 0.0F );
        bgm->set_loop_end_second( 67.227F );
        bgm->set_name( "bgm" );
        bgm->set_loop( true );
        bgm->play( );
        dont_deatroy_node.lock( )->add_child( bgm );
    }

    auto tit = create_dot( "title.png", app::getWindowWidth( ) * 0.9F );
    _title_logo = tit;
    add_child( tit );
    tit->set_position( vec2( app::getWindowSize( ) ) * vec2( 0.5F, 0.35F ) );

    auto sta = create_dot_button( "start.png", 200 );
    {
        auto sound = ::audio::buffer_player::create( "sound/start.wav" );
        sound->set_name( "sound" );
        sta->add_child( sound );
    }
    _start_button = sta;
    add_child( sta );
    sta->set_position( vec2( app::getWindowSize( ) ) * vec2( 0.5F, 0.75F ) );

    auto gac = create_dot_button( "gacha.png", 100 );
    {
        auto sound = ::audio::buffer_player::create( "sound/gacha.wav" );
        sound->set_name( "sound" );
        gac->add_child( sound );
    }
    _gacha_button = gac;
    add_child( gac );
    gac->set_position( vec2( app::getWindowSize( ) ) * vec2( 1.0F, 0.75F ) + vec2( -150, -60 ) );

    auto con = create_dot_button( "config.png", 100 );
    {
        auto sound = ::audio::buffer_player::create( "sound/config.wav" );
        sound->set_name( "sound" );
        con->add_child( sound );
    }
    _config_button = con;
    add_child( con );
    con->set_position( vec2( app::getWindowSize( ) ) * vec2( 1.0F, 0.75F ) + vec2( -150, 60 ) );

    sta->on_ended = [ this ]
    {
        std::dynamic_pointer_cast<::audio::buffer_player> ( _start_button.lock( )->get_child_by_name( "sound" ) )->play( );
        set_block_schedule_event( );
        auto search_handle = search_room::create( );
        search_handle->on_founded = [ this ] ( Json::Value& root, std::map<int, cinder::ivec2>& feeds_buffer, std::vector<std::vector<unsigned char>>& ground_buffer )
        {
            change_action( [ &root, &feeds_buffer, &ground_buffer ]
            {
                scene_manager::get_instans( )->replace( game::create( root, feeds_buffer, ground_buffer ) );
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
        std::dynamic_pointer_cast<::audio::buffer_player> ( _gacha_button.lock( )->get_child_by_name( "sound" ) )->play( );
        set_block_schedule_event( );
        change_action( [ ] { scene_manager::get_instans( )->replace( gacha::create( ) ); } );
    };
    con->on_ended = [ this ]
    {
        std::dynamic_pointer_cast<::audio::buffer_player> ( _config_button.lock( )->get_child_by_name( "sound" ) )->play( );
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