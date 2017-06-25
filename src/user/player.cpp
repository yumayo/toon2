#include "player.h"
#include "action.hpp"
#include "utility/string_utility.h"
#include "skin.h"
#include "scene_manager.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player, std::string const& ip_address,
            int port, std::string const& relative_path_skin )
{
    CREATE( player, ip_address, port, relative_path_skin );
}
bool player::init( std::string const& ip_address,
                   int port, std::string const& relative_path_skin )
{
    if ( !cell::init( ip_address, port, relative_path_skin ) ) return false;
    set_schedule_key_event( );
    _target_radius = _radius;
    return true;
}
player::~player( )
{
    auto feed_num = user_default::get_instans( )->get_root( )["feed"].asInt( );
    user_default::get_instans( )->get_root( )["feed"] = feed_num + int( _radius - _setup_radius );
    auto max_radius = user_default::get_instans( )->get_root( )["max_radius"].asFloat( );
    user_default::get_instans( )->get_root( )["max_radius"] = std::max( max_radius, _radius );
}
void player::key_down( cinder::app::KeyEvent e )
{
    if ( e.getCode( ) == cinder::app::KeyEvent::KEY_b )
    {
        blowout( );
    }
}
void player::on_captured( std::weak_ptr<node> other )
{
    Json::Value root;
    root["name"] = "player_on_captured";
    root["data"]["score"] = _radius;
    root["data"]["id"] = other.lock( )->get_tag( );
    _tcp_connection.lock( )->write( Json::FastWriter( ).write( root ) );
}
void player::capture( float score )
{
    scale_action( score );
}
void player::scale_action( float score )
{
    remove_action_by_name( "scale_action" );

    _target_radius = std::max( 10.0F, _target_radius + score );

    auto sub = _target_radius - _radius;
    auto t = 2.0F + easeOutCubic( clamp( sub, 0.0F, 50.0F ) / 50.0F ) * 4.0F;
    auto act = action::ease<EaseOutSine>::create( action::float_to::create( t, _radius, _target_radius, [ this ] ( float value )
    {
        set_radius( value );
    }
    ) );
    act->set_name( "scale_action" );
    run_action( act );
}
void player::blowout( )
{
    // TODO: フィールドに自分と同じ色のエサを撒き散らす。

    scale_action( -20.0F );
}
}
