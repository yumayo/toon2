#include "player.h"
#include <treelike/action.hpp>
#include "skin.h"
#include <treelike/scene_manager.h>
#include "bullet_manager.h"
#include "cell_manager.h"
#include <treelike/user_default.h>
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( player, softptr<node> cell_manager, std::string const& ip_address,
            int port, std::string const& relative_path_skin )
{
    CREATE( player, cell_manager, ip_address, port, relative_path_skin );
}
bool player::init( softptr<node> cell_manager, std::string const& ip_address,
                   int port, std::string const& relative_path_skin )
{
    if ( !cell::init( ip_address, port, relative_path_skin ) ) return false;
    _target_radius = _radius;
    _cell_manager = cell_manager;
    return true;
}
player::~player( )
{
    auto min_radius = user_default::get_instans( )->get_root( )["min_radius"].asFloat( );
    auto max_radius = user_default::get_instans( )->get_root( )["max_radius"].asFloat( );
    auto feed_num = user_default::get_instans( )->get_root( )["feed"].asInt( );
    user_default::get_instans( )->get_root( )["feed"] = feed_num + int( _radius - min_radius );
    user_default::get_instans( )->get_root( )["max_radius"] = std::max( max_radius, _radius );
}
void player::on_captured( softptr<node> other )
{
    Json::Value root;
    root["name"] = "player_on_captured";
    root["data"]["score"] = _radius;
    root["data"]["id"] = other->get_tag( );
    _tcp_connection->write( Json::FastWriter( ).write( root ) );
}
void player::capture( float score )
{
    scale_action( score );
}
void player::scale_action( float score )
{
    remove_action_by_name( "scale_action" );

    _target_radius = glm::clamp( _target_radius + score,
                                 user_default::get_instans( )->get_root( )["system"]["min_radius"].asFloat( ),
                                 user_default::get_instans( )->get_root( )["system"]["max_radius"].asFloat( ) );

    auto sub = _target_radius - _radius;
    auto t = 1.5F + easeOutCubic( clamp( sub, 0.0F, 50.0F ) / 50.0F ) * 1.5F;
    auto act = action::ease<EaseOutSine>::create( action::float_to::create( t, _radius, _target_radius, [ this ] ( float value )
    {
        set_radius( value );
    }
    ) );
    act->set_name( "scale_action" );
    run_action( act );
}
void player::blowout( float power )
{
    scale_action( -power );

    if ( _radius < user_default::get_instans( )->get_root( )["system"]["min_radius"].asFloat( ) )
    {
        Json::Value root;
        root["name"] = "kill";
        _tcp_connection->write( Json::FastWriter( ).write( root ) );
    }
}
float player::get_target_radius( )
{
    return _target_radius;
}
}
