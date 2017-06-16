#include "feed_manager_client.h"
#include "feed.h"
#include "player_manager.h"
using namespace cinder;
namespace user
{
CREATE_CPP( feed_manager_client, std::weak_ptr<node> player_manager )
{
    CREATE( feed_manager_client, player_manager );
}
bool feed_manager_client::init( std::weak_ptr<node> player_manager )
{
    if ( !feed_manager::init( player_manager ) ) return false;

    return true;
}
void feed_manager_client::update( float delta )
{
    auto pla_manager = std::dynamic_pointer_cast<player_manager>( _player_manager.lock( ) );
    std::vector<std::weak_ptr<player>> players;
    players.emplace_back( pla_manager->get_player( ) );
    for ( auto& c : pla_manager->get_clients( ) ) players.emplace_back( std::dynamic_pointer_cast<player>( c ) );

    for ( auto p : players )
    {
        for ( auto& child : _player_manager.lock( )->get_children( ) )
        {
            for ( auto& f : _children )
            {
                auto fee = std::dynamic_pointer_cast<feed>( f );
                if ( fee->is_captureing( ) ) continue;

                // 自分の半径の二倍分の距離から吸い取れます。
                if ( distance( fee->get_position( ), p.lock( )->get_position( ) ) < fee->get_radius( ) * 2 + p.lock( )->get_radius( ) )
                {
                    // 取得したオブジェクトをサーバーに送ります。
                    Json::Value root;
                    root["name"] = "feed_captured";
                    root["data"]["tag"] = fee->get_tag( );
                    _udp_connection.lock( )->write( pla_manager->get_host_handle( ), root );

                    fee->captured( p.lock( ) );
                }
            }
        }
    }
}
void feed_manager_client::create_feed( Json::Value const & root )
{
    auto f = feed::create( );
    f->set_tag( root["tag"].asInt( ) );
    f->set_position( vec2( root["position"][0].asFloat( ), root["position"][1].asFloat( ) ) );
    add_child( f );
}
}

