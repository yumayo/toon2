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
    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _tcp_connection = std::dynamic_pointer_cast<network::tcp_client>( dont_destroy_node.lock( )->get_child_by_name( "tcp_connection" ) );

    _handle = std::make_shared<network::network_object>( ip_address, port );

    _setup_radius = 20.0F;

    _radius = _setup_radius;
    if ( auto base = renderer::circle::create( _radius ) )
    {
        _base = base;
        add_child( base );

        if ( auto mask = relative_path_skin.empty( ) ?
             renderer::circle::create( _radius ) : skin::create( _radius, relative_path_skin ) )
        {
            _mask = mask;
            mask->set_scale( vec2( 0.9F ) );
            base->add_child( mask );
        }
    }

    return true;
}
player::~player( )
{
    if ( get_name( ) == "player" )
    {
        auto feed_num = user_default::get_instans( )->get_root( )["feed"].asInt( );
        user_default::get_instans( )->get_root( )["feed"] = feed_num + int( _radius - _setup_radius );
    }
}
float player::get_radius( )
{
    return _radius;
}
float player::get_startup_radius( )
{
    return _setup_radius;
}
void player::set_radius( float value )
{
    _radius = value;
    _base.lock( )->set_radius( get_radius( ) );
    _mask.lock( )->set_radius( get_radius( ) );

    if ( auto crown = _base.lock( )->get_child_by_name( "crown" ) )
    {
        crown->set_scale( vec2( _radius / ( crown->get_content_size( ).x / 2 ) ) );
    }
}
void player::on_captured( std::weak_ptr<node> other )
{
    if ( get_name( ) == "player" )
    {
        Json::Value root;
        root["name"] = "player_on_captured";
        root["data"]["score"] = _radius;
        root["data"]["id"] = other.lock( )->get_tag( );
        _tcp_connection.lock( )->write( Json::FastWriter( ).write( root ) );
    }
}
void player::capture( float score )
{
    remove_action_by_name( "capture_animation" );

    // どの大きさまで大きくなるのかを設定する。
    _target_radius = _radius + score;

    // アクションは終了済み。
    auto sub = _target_radius - _radius;
    auto t = 0.2F + easeOutCubic( clamp( sub, 0.0F, 50.0F ) / 50.0F ) * 4.8F;
    auto act = action::ease<EaseOutSine>::create( action::float_to::create( t, _radius, _target_radius, [ this ] ( float value )
    {
        set_radius( value );
    }
    ) );
    act->set_name( "capture_animation" );
    run_action( act );
}
void user::player::set_color( cinder::ColorA value )
{
    node::set_color( value );
    _mask.lock( )->set_color( value );
}
void player::remove_crown( )
{
    if ( auto crown = _base.lock( )->get_child_by_name( "crown" ) )
    {
        crown->remove_from_parent( );
    }
}
void player::set_crown( std::weak_ptr<node> crown )
{
    crown.lock( )->set_name( "crown" );
    _base.lock( )->add_child( crown.lock( ) );

    if ( auto crown = _base.lock( )->get_child_by_name( "crown" ) )
    {
        crown->set_scale( vec2( _radius / ( crown->get_content_size( ).x / 2 ) ) );
    }
}
network::network_handle user::player::get_handle( )
{
    return _handle;
}
}
