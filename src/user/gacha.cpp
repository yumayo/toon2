#include "gacha.h"
#include "create_dot_obeject.h"
#include "scene_manager.h"
#include "title.h"
#include "feed.h"
#include "dot_label.h"
#include "boost/lexical_cast.hpp"
#include "player.h"
#include "cinder/Rand.h"
#include "se.h"
#include "utility/file_system.h"
using namespace cinder;
namespace user
{
CREATE_CPP( gacha )
{
    CREATE( gacha );
}
bool gacha::init( )
{
    utility::file_system system;
    system.search( app::getAssetPath( "skin/" ).string( ) );
    _skin_items = system.get_names( );
    // 拡張子を取り除く。
    for ( auto& i : _skin_items ) i = i.substr( 0, i.rfind( '.' ) );
    auto& root = user_default::get_instans( )->get_root( );
    for ( auto const& i : _skin_items )
    {
        root["skin"][i];
    }

    set_schedule_mouse_event( );
    set_schedule_touch_event( );

    auto mas = create_dot( "gacha_maschine.png", 600 );
    _maschine = mas;
    mas->set_position( vec2( app::getWindowSize( ) ) * 0.5F );
    add_child( mas );

    auto gar = create_dot_button( "garagara.png", 200 );
    _garagara = gar;
    gar->set_position( vec2( app::getWindowSize( ) ) * 0.5F + vec2( 0, -100 ) );
    using namespace action;
    gar->run_action( repeat_forever::create( sequence::create( ease<EaseInOutSine>::create( move_by::create( 1.0F, vec2( 0, -20 ) ) ),
                                                               ease<EaseInOutSine>::create( move_by::create( 1.0F, vec2( 0, 20 ) ) ) ) ) );
    // コンプしてなかったらガチャマークを登録します。
    if ( !is_complete( ) )  add_child( gar );

    auto bac = create_dot_button( "back.png", 150 );
    _back_button = bac;
    bac->set_position( vec2( app::getWindowSize( ) ) * vec2( 0, 1 ) + vec2( 100, -100 ) );
    add_child( bac );

    auto feed_num = user_default::get_instans( )->get_root( )["feed"].asInt( );

    auto fla = renderer::label::create( "x" + boost::lexical_cast<std::string>( feed_num ), "misaki_gothic.ttf", 86 );
    _feed_number_label = fla;
    fla->set_position( vec2( app::getWindowSize( ) ) * vec2( 1, 1 ) + vec2( -50, -50 ) );
    fla->set_anchor_point( vec2( 1, 1 ) );
    fla->set_pivot( vec2( 0, 0.6 ) );
    add_child( fla );

    auto fee = feed::create( node::INVALID_TAG, vec2( 0 ) );
    fee->set_position( vec2( -50, 0 ) );
    fla->add_child( fee );

    auto m = fee->get_world_matrix( );
    m = translate( m, fee->get_position( ) );
    auto start_pos = vec2( m[2][0], m[2][1] );
    auto end_pos = gar->get_position( );
    auto mas_pos = mas->get_position( );

    gar->on_ended = [ this, start_pos, end_pos, mas_pos ]
    {
        auto& root = user_default::get_instans( )->get_root( );
        if ( root["feed"].asInt( ) < 10 ) return;

        play_se( "sound/garagara.wav" );
        set_block_schedule_event( );
        auto spawn = call_func::create( [ this, start_pos, end_pos, &root ]
        {
            auto feed_num = root["feed"].asInt( ) - 1;
            root["feed"] = feed_num;
            _feed_number_label.lock( )->set_text( "x " + boost::lexical_cast<std::string>( feed_num ) );
            auto f = feed::create( node::INVALID_TAG, vec2( 0 ) );
            f->set_position( start_pos );
            f->run_action( sequence::create( ease<EaseInOutCirc>::create( move_to::create( 1.0F, end_pos ) ), remove_self::create( ) ) );
            add_child( f );
        } );
        auto new_skin = call_func::create( [ this, &root, mas_pos ]
        {
            play_se( "sound/view.wav" );
            auto eff = renderer::sprite_animation::create( "new_skin_effect.png" );
            eff->set_name( "effect" );
            eff->set_cut( { 8, 7 } );
            eff->set_cut_size( vec2( 128 ) );
            eff->set_position( mas_pos + vec2( 0, 200 ) );
            eff->run_action( ease<EaseOutExpo>::create( scale_to::create( 0.5F, vec2( 7 ) ) ) );
            eff->run_action( sequence::create( ease<EaseOutExpo>::create( move_to::create( 0.75F, vec2( app::getWindowSize( ) ) * vec2( 0.5F ) ) ), call_func::create( [ this ] { _is_animation_end = true; } ) ) );
            add_child( eff );

            auto player = player::create( "", 0, "skin/" + get_new_skin_name( ) + ".png" );
            player->run_action( repeat_forever::create( rotate_by::create( 10.0F, M_PI * 2 ) ) );
            eff->add_child( player );

            if ( is_complete( ) ) remove_child( _garagara.lock( ) );
        } );
        run_action( sequence::create( repeat_times::create( sequence::create( spawn, delay::create( 0.05F ) ), 10 ), delay::create( 1.0F ), new_skin ) );
    };
    bac->on_ended = [ this ]
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
        eff->run_action( action::sequence::create( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.3F, vec2( 0 ) ) ),
                                                   action::remove_self::create( ) ) );
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
        eff->run_action( action::sequence::create( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.3F, vec2( 0 ) ) ),
                                                   action::remove_self::create( ) ) );
        set_block_schedule_event( false );
    }
    return true;
}
void gacha::change_action( std::function<void( )> end_fn )
{
    float i = 0;
    size_t size = _maschine.lock( )->get_children( ).size( );
    for ( auto& c : _maschine.lock( )->get_children( ) )
    {
        c->remove_all_actions( );
        c->run_action( action::sequence::create(
            action::delay::create( i ),
            action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    if ( _garagara.lock( ) )
    {
        i = 0;
        size = _garagara.lock( )->get_children( ).size( );
        for ( auto& c : _garagara.lock( )->get_children( ) )
        {
            c->remove_all_actions( );
            c->run_action( action::sequence::create(
                action::delay::create( i ),
                action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
            i += 1.0F / size;
        }
    }
    i = 0;
    size = _back_button.lock( )->get_children( ).size( );
    auto itr = _back_button.lock( )->get_children( ).begin( );
    for ( ; itr != --_back_button.lock( )->get_children( ).end( ); ++itr )
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

