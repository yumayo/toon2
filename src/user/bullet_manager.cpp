#include "bullet_manager.h"
#include "cell_manager.h"
#include "scene_manager.h"
#include "bullet.h"
#include "utility.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( bullet_manager, std::weak_ptr<node> cell_manager )
{
    CREATE( bullet_manager, cell_manager );
}
bool bullet_manager::init( std::weak_ptr<node> cell_manager )
{
    set_schedule_update( );

    set_name( "bullet_manager" );

    _cell_manager = cell_manager;

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _tcp_connection = std::dynamic_pointer_cast<network::tcp_client>( dont_destroy_node.lock( )->get_child_by_name( "tcp_connection" ) );

    _tcp_connection.lock( )->on_received_named_json.insert( std::make_pair( "create_bullet", [ this ] ( Json::Value root )
    {
        auto tag = root["data"]["tag"].asInt( );
        auto id = root["data"]["id"].asInt( );
        auto pos = vec2( root["data"]["position"][0].asFloat( ), root["data"]["position"][1].asFloat( ) );
        auto direction = vec2( root["data"]["direction"][0].asFloat( ), root["data"]["direction"][1].asFloat( ) );
        if ( auto cell = std::dynamic_pointer_cast<user::cell>( _cell_manager.lock( )->get_child_by_tag( id ) ) )
        {
            add_child( bullet::create( tag, pos, direction, cell ) );
        }
        else
        {
            utility::log( "【bullet_manager】対象のプレイヤー[%d]が見つかりませんでした。", id );
        }
    } ) );

    return true;
}
void bullet_manager::update( float delta )
{
    auto cell_manager = std::dynamic_pointer_cast<user::cell_manager>( _cell_manager.lock( ) );
    auto player = cell_manager->get_player( );
    for ( auto const& c : get_children( ) )
    {
        if ( c->get_color( ) == player.lock( )->get_color( ) ) continue;
        if ( auto const& bullet = std::dynamic_pointer_cast<user::bullet>( c ) )
        {
            if ( distance( player.lock( )->get_position( ), bullet->get_position( ) ) 
                 < player.lock( )->get_radius( ) + bullet->get_radius( ) )
            {
                player.lock( )->blowout( );
            }
        }
    }
}
void bullet_manager::close_player( cinder::ColorA const& color )
{
    for ( auto const& c : get_children( ) )
    {
        if ( c->get_color( ) == color )
        {
            c->remove_from_parent( );
        }
    }
}
}

