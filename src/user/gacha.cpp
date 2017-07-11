#include "gacha.h"
#include "create_dot_obeject.h"
#include <treelike/scene_manager.h>
#include "title.h"
#include "feed.h"
#include "boost/lexical_cast.hpp"
#include "player.h"
#include "cinder/Rand.h"
#include "se.h"
#include <treelike/utility/file_system.h>
#include "enemy.h"
#include <cinder/gl/gl.h>
#include "render3d.h"
using namespace cinder;
using namespace treelike;
using namespace treelike::action;
namespace user
{
CREATE_CPP( gacha )
{
    CREATE( gacha );
}
bool gacha::init( )
{
    set_schedule_mouse_event( );
    set_schedule_touch_event( );

    _3d = add_child( render3d::create( ) );

    utility::file_system system;
    system.search( app::getAssetPath( "skin/" ).string( ) );
    _skin_items = system.get_names( );
    // 拡張子を取り除く。
    for ( auto& i : _skin_items ) i = i.substr( 0, i.rfind( '.' ) );
    auto& root = user_default::get_instans( )->get_root( );
    for ( auto const& i : _skin_items )
    {
        if ( i == "null" ) continue; // nullテクスチャ読み込みません。
        root["skin"][i];
    }

    _maschine = _3d->add_child( dot_object::create( "gacha_maschine.png", 600 ) );
    _maschine->set_position( vec2( app::getWindowSize( ) ) * 0.5F );

    if ( !is_complete( ) )
    {
        _garagara = _3d->add_child( dot_button::create( "garagara.png", 200 ) );
        _garagara->set_position( vec2( app::getWindowSize( ) ) * 0.5F + vec2( 0, -100 ) );
        _garagara->run_action( repeat_forever::create( sequence::create( ease<EaseInOutSine>::create( move_by::create( 1.0F, vec2( 0, -20 ) ) ),
                                                                         ease<EaseInOutSine>::create( move_by::create( 1.0F, vec2( 0, 20 ) ) ) ) ) );
    }

    _back_button = _3d->add_child( dot_button::create( "back.png", 150 ) );
    _back_button->set_position( vec2( app::getWindowSize( ) ) * vec2( 0, 1 ) + vec2( 100, -100 ) );

    auto feed_num = user_default::get_instans( )->get_root( )["feed"].asInt( );

    _feed_number_label = add_child( renderer::label::create( "misaki_gothic.ttf", 86 ) );
    _feed_number_label->set_text( "x" + boost::lexical_cast<std::string>( feed_num ) );
    _feed_number_label->set_position( vec2( app::getWindowSize( ) ) * vec2( 1, 1 ) + vec2( -50, -50 ) );
    _feed_number_label->set_anchor_point( vec2( 1, 1 ) );
    _feed_number_label->set_pivot( vec2( 0, 0.6 ) );


    _feed = _feed_number_label->add_child( feed::create( node::INVALID_TAG, vec2( 0 ) ) );
    _feed->set_position( vec2( -50, 0 ) );

    _garagara.dynamicptr<dot_button>( )->on_ended = [ this ]
    {
        auto m = _feed->get_world_matrix( );
        m = translate( m, _feed->get_position( ) );
        auto start_pos = vec2( m[2][0], m[2][1] );
        auto end_pos = _garagara->get_position( );
        auto mas_pos = _maschine->get_position( );

        if ( user_default::get_instans( )->get_root( )["feed"].asInt( ) < 10 ) return;

        play_se( "sound/garagara.wav" );
        set_block_schedule_event( );
        auto spawn = call_func::create( [ this, start_pos, end_pos ]
        {
            auto feed_num = user_default::get_instans( )->get_root( )["feed"].asInt( ) - 1;
            user_default::get_instans( )->get_root( )["feed"] = feed_num;
            _feed_number_label->set_text( "x" + boost::lexical_cast<std::string>( feed_num ) );
            auto f = feed::create( node::INVALID_TAG, vec2( 0 ) );
            f->set_position( start_pos );
            f->run_action( sequence::create( ease<EaseInOutCirc>::create( move_to::create( 1.0F, end_pos ) ), remove_self::create( ) ) );
            add_child( f );
        } );
        auto new_skin = call_func::create( [ this, mas_pos ]
        {
            play_se( "sound/view.wav" );
            auto effect = renderer::sprite_animation::create( "new_skin_effect.png" );
            effect->set_name( "effect" );
            effect->set_cut( { 8, 7 } );
            effect->set_cut_size( vec2( 128 ) );
            effect->set_position( mas_pos + vec2( 0, 200 ) );
            effect->run_action( ease<EaseOutExpo>::create( scale_to::create( 0.5F, vec2( 7 ) ) ) );
            effect->run_action( sequence::create( ease<EaseOutExpo>::create( move_to::create( 0.75F, vec2( app::getWindowSize( ) ) * vec2( 0.5F ) ) ), call_func::create( [ this ] { _is_animation_end = true; } ) ) );
            add_child( effect );

            auto enemy = enemy::create( "", 0, "skin/" + get_new_skin_name( ) + ".png" );
            enemy->run_action( repeat_forever::create( rotate_by::create( 10.0F, M_PI * 2 ) ) );
            effect->add_child( enemy );

            if ( is_complete( ) ) _3d->remove_child( _garagara );
        } );
        run_action( sequence::create( repeat_times::create( sequence::create( spawn, delay::create( 0.05F ) ), 10 ), delay::create( 1.0F ), new_skin ) );
    };
    _back_button.dynamicptr<dot_button>( )->on_ended = [ this ]
    {
        play_se( "sound/back.wav" );
        set_block_schedule_event( );
        change_action( [ ] { scene_manager::get_instans( )->replace( title::create( ) ); } );
    };

    return true;
}
bool gacha::mouse_began( cinder::app::MouseEvent event )
{
    if ( !_is_animation_end ) return false;
    if ( auto eff = get_child_by_name( "effect" ) )
    {
        _is_animation_end = false;
        eff->run_action( sequence::create( ease<EaseOutBounce>::create( scale_to::create( 0.3F, vec2( 0 ) ) ),
                                           remove_self::create( ) ) );
        set_block_schedule_event( false );
    }
    return true;
}
bool gacha::touch_began( cinder::app::TouchEvent::Touch event )
{
    if ( !_is_animation_end ) return false;
    if ( auto eff = get_child_by_name( "effect" ) )
    {
        _is_animation_end = false;
        eff->run_action( sequence::create( ease<EaseOutBounce>::create( scale_to::create( 0.3F, vec2( 0 ) ) ),
                                           remove_self::create( ) ) );
        set_block_schedule_event( false );
    }
    return true;
}
void gacha::change_action( std::function<void( )> end_fn )
{
    float i = 0;
    size_t size = _maschine->get_children( ).size( );
    for ( auto& c : _maschine->get_children( ) )
    {
        c->remove_all_actions( );
        c->run_action( sequence::create(
            delay::create( i ),
            ease<EaseOutBounce>::create( scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    if ( _garagara )
    {
        i = 0;
        size = _garagara->get_children( ).size( );
        for ( auto& c : _garagara->get_children( ) )
        {
            c->remove_all_actions( );
            c->run_action( sequence::create(
                delay::create( i ),
                ease<EaseOutBounce>::create( scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
            i += 1.0F / size;
        }
    }
    i = 0;
    size = _back_button->get_children( ).size( );
    auto itr = _back_button->get_children( ).begin( );
    for ( ; itr != --_back_button->get_children( ).end( ); ++itr )
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
std::string user::gacha::get_new_skin_name( )
{
    auto& root = user_default::get_instans( )->get_root( );
    auto& skin = root["skin"];
    std::vector<std::string> no_get_skins;
    for ( auto& name : skin.getMemberNames( ) )
    {
        if ( !skin[name].asBool( ) )
            no_get_skins.emplace_back( name );
    }

    Rand rand( app::getElapsedSeconds( ) * 1000 );
    auto index = rand.randInt( 0, no_get_skins.size( ) );

    std::string new_skin_name = no_get_skins[index];

    skin[new_skin_name] = true;

    return new_skin_name;
}
bool gacha::is_complete( )
{
    auto& root = user_default::get_instans( )->get_root( );
    auto& skin = root["skin"];
    for ( auto const& i : _skin_items )
    {
        if ( !skin[i].asBool( ) ) return false;
    }
    return true;
}
}

