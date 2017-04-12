#include "title.h"
#include "create_dot_obeject.h"
#include "../scene_manager.h"
#include "game.h"
#include "gacha.h"
#include "config.h"
#include "search_room.h"
using namespace cinder;
namespace user
{
CREATE_CPP( title )
{
    CREATE( title );
}
bool title::init( )
{
    auto tit = create_dot( "title.png", app::getWindowWidth( ) * 0.9F );
    _tit = tit;
    add_child( tit );
    tit->set_position( vec2( app::getWindowSize( ) ) * vec2( 0.5F, 0.35F ) );

    auto sta = create_dot_button( "start.png", 200 );
    _sta = sta;
    add_child( sta );
    sta->set_position( vec2( app::getWindowSize( ) ) * vec2( 0.5F, 0.75F ) );

    auto gac = create_dot_button( "gacha.png", 100 );
    _gac = gac;
    add_child( gac );
    gac->set_position( vec2( app::getWindowSize( ) ) * vec2( 1.0F, 0.75F ) + vec2( -150, -60 ) );

    auto con = create_dot_button( "config.png", 100 );
    _con = con;
    add_child( con );
    con->set_position( vec2( app::getWindowSize( ) ) * vec2( 1.0F, 0.75F ) + vec2( -150, 60 ) );

    sta->on_ended = [ this ]
    {
        set_block_schedule_event( );
        auto search_handle = search_room::create( );
        search_handle->on_founded = [ this ] ( Json::Value& root )
        {
            change_action( [ root ] { scene_manager::get_instans( )->replace( game::create( root ) ); } );
        };
        search_handle->on_not_found = [ this ] ( )
        {
            set_block_schedule_event(  false );
        };
        add_child( search_handle );
    };
    gac->on_ended = [ this ]
    {
        set_block_schedule_event( );
        change_action( [ ] { scene_manager::get_instans( )->replace( gacha::create( ) ); } );
    };
    con->on_ended = [ this ]
    {
        set_block_schedule_event( );
        change_action( [ ] { scene_manager::get_instans( )->replace( config::create( ) ); } );
    };

    return true;
}
void title::change_action( std::function<void( )> end_fn )
{
    float i = 0;
    size_t size = _tit.lock( )->get_children( ).size( );
    for ( auto& c : _tit.lock( )->get_children( ) )
    {
        c->remove_all_actions( );
        c->run_action( action::sequence::create(
            action::delay::create( i ),
            action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    i = 0;
    size = _sta.lock( )->get_children( ).size( );
    for ( auto& c : _sta.lock( )->get_children( ) )
    {
        c->remove_all_actions( );
        c->run_action( action::sequence::create(
            action::delay::create( i ),
            action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    i = 0;
    size = _gac.lock( )->get_children( ).size( );
    for ( auto& c : _gac.lock( )->get_children( ) )
    {
        c->remove_all_actions( );
        c->run_action( action::sequence::create(
            action::delay::create( i ),
            action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    i = 0;
    size = _con.lock( )->get_children( ).size( );
    auto itr = _con.lock( )->get_children( ).begin( );
    for ( ; itr != --_con.lock( )->get_children( ).end( ); ++itr )
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