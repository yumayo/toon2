#include "gacha.h"
#include "create_dot_obeject.h"
#include "scene_manager.h"
#include "title.h"
#include "feed.h"
#include "dot_label.h"
#include "boost/lexical_cast.hpp"
#include "player.h"
#include "cinder/Rand.h"
using namespace cinder;
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

    auto mas = create_dot( "gacha_maschine.png", 600 );
    _mas = mas;
    mas->set_position( vec2( app::getWindowSize( ) ) * 0.5F );
    add_child( mas );

    auto& root = user_default::get_instans( )->get_root( );

    auto gar = create_dot_button( "garagara.png", 200 );
    _gar = gar;
    gar->set_position( vec2( app::getWindowSize( ) ) * 0.5F + vec2( 0, -100 ) );
    using namespace action;
    gar->run_action( repeat_forever::create( sequence::create( ease<EaseInOutSine>::create( move_by::create( 1.0F, vec2( 0, -20 ) ) ),
                                                               ease<EaseInOutSine>::create( move_by::create( 1.0F, vec2( 0, 20 ) ) ) ) ) );

    if ( !root["complete"].asBool( ) )
    {
        add_child( gar );
    }

    auto bac = create_dot_button( "back.png", 150 );
    _bac = bac;
    bac->set_position( vec2( app::getWindowSize( ) ) * vec2( 0, 1 ) + vec2( 100, -100 ) );
    add_child( bac );


    auto feed_num = root["feed"].asInt( );

    auto fla = renderer::label::create( "x" + boost::lexical_cast<std::string>( feed_num ), "misaki_gothic.ttf", 86 );
    _fla = fla;
    fla->set_position( vec2( app::getWindowSize( ) ) * vec2( 1, 1 ) + vec2( -50, -50 ) );
    fla->set_anchor_point( vec2( 1, 1 ) );
    fla->set_pivot( vec2( 0, 0.6 ) );
    add_child( fla );

    auto fee = feed::create( );
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

        set_block_schedule_event( );
        auto spawn = call_func::create( [ this, start_pos, end_pos, &root ]
        {
            auto feed_num = root["feed"].asInt( ) - 1;
            root["feed"] = feed_num;
            _fla.lock( )->set_text( "x " + boost::lexical_cast<std::string>( feed_num ) );
            auto f = feed::create( );
            f->set_position( start_pos );
            f->run_action( sequence::create( ease<EaseInOutCirc>::create( move_to::create( 1.0F, end_pos ) ), remove_self::create( ) ) );
            add_child( f );
        } );
        auto new_skin = call_func::create( [ this, &root, mas_pos ]
        {
            auto eff = renderer::sprite_animation::create( "new_skin_effect.png" );
            eff->set_name( "effect" );
            eff->set_cut( { 8, 7 } );
            eff->set_cut_size( vec2( 128 ) );
            eff->set_position( mas_pos + vec2( 0, 200 ) );
            eff->run_action( ease<EaseOutExpo>::create( scale_to::create( 0.5F, vec2( 7 ) ) ) );
            eff->run_action( ease<EaseOutExpo>::create( move_to::create( 0.75F, vec2( app::getWindowSize( ) ) * vec2( 0.5F ) ) ) );
            add_child( eff );

            auto pla = player::create( "skin/" + get_new_skin_name( ) + ".png" );
            pla->run_action( repeat_forever::create( rotate_by::create( 10.0F, M_PI * 2 ) ) );
            eff->add_child( pla );

            if ( root["complete"].asBool( ) ) remove_child( _gar.lock( ) );
        } );
        run_action( sequence::create( repeat_times::create( sequence::create( spawn, delay::create( 0.05F ) ), 10 ), new_skin ) );
    };
    bac->on_ended = [ this ]
    {
        set_block_schedule_event( );
        change_action( [ ] { scene_manager::get_instans( )->replace( title::create( ) ); } );
    };

    return true;
}
bool gacha::mouse_began( cinder::app::MouseEvent event )
{
    if ( auto eff = get_child_by_name( "effect" ) )
    {
        eff->run_action( action::sequence::create( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.3F, vec2( 0 ) ) ),
                                                   action::remove_self::create( ) ) );
        set_block_schedule_event( false );
    }
    return true;
}
bool gacha::touch_began( cinder::app::TouchEvent::Touch event )
{
    if ( auto eff = get_child_by_name( "effect" ) )
    {
        eff->run_action( action::sequence::create( action::ease<EaseOutBounce>::create( action::scale_to::create( 0.3F, vec2( 0 ) ) ),
                                                   action::remove_self::create( ) ) );
        set_block_schedule_event( false );
    }
    return true;
}
void gacha::change_action( std::function<void( )> end_fn )
{
    float i = 0;
    size_t size = _mas.lock( )->get_children( ).size( );
    for ( auto& c : _mas.lock( )->get_children( ) )
    {
        c->remove_all_actions( );
        c->run_action( action::sequence::create(
            action::delay::create( i ),
            action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
        i += 1.0F / size;
    }
    if ( _gar.lock( ) )
    {
        i = 0;
        size = _gar.lock( )->get_children( ).size( );
        for ( auto& c : _gar.lock( )->get_children( ) )
        {
            c->remove_all_actions( );
            c->run_action( action::sequence::create(
                action::delay::create( i ),
                action::ease<EaseOutBounce>::create( action::scale_to::create( 0.2F, vec2( 0 ) ) ) ) );
            i += 1.0F / size;
        }
    }
    i = 0;
    size = _bac.lock( )->get_children( ).size( );
    auto itr = _bac.lock( )->get_children( ).begin( );
    for ( ; itr != --_bac.lock( )->get_children( ).end( ); ++itr )
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

    if ( no_get_skins.size( ) == 1 ) root["complete"] = true;

    return new_skin_name;
}
}

