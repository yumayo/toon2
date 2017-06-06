#include "player_manager_client.h"
#include "utility/string_utility.h"
#include "utility/assert_log.h"
#include "boost/lexical_cast.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager_client, Json::Value& root, std::shared_ptr<network::udp_connection>& connection )
{
    CREATE( player_manager_client, root, connection );
}
bool player_manager_client::init( Json::Value& root, std::shared_ptr<network::udp_connection>& connection )
{
    if ( !player_manager::init( root, connection ) ) return false;

    // 他のオブジェクトからゲームの更新命令がされたら呼ばれます。
    _udp.lock( )->on_received_named_json.insert( std::make_pair( "game_update", [ this ] ( network::network_handle handle, Json::Value root )
    {
        for ( auto& child : _clients.lock( )->get_children( ) )
        {
            std::weak_ptr<player> client = std::dynamic_pointer_cast<player>( child );
            client.lock( )->set_position( vec2( root["data"]["position"][0].asFloat( ),
                                                root["data"]["position"][1].asFloat( ) ) );
            client.lock( )->set_radius( root["data"]["radius"].asFloat( ) );
        }
    } ) );

    return true;
}
void player_manager_client::update( float delta )
{
    if ( !_player.lock( ) || _player.expired( ) ) return;

    // 大きさで、描画順を変更。
    _clients.lock( )->get_children( ).sort( [ ] ( std::shared_ptr<node>& a, std::shared_ptr<node>& b )
    {
        std::weak_ptr<player> p_a = std::dynamic_pointer_cast<player>( a );
        std::weak_ptr<player> p_b = std::dynamic_pointer_cast<player>( b );
        return p_a.lock( )->get_radius( ) < p_b.lock( )->get_radius( );
    } );

    // プレイヤー同士の当たり判定。
    // 他のエネミー同士は計算しません。
    for ( auto& child : _clients.lock( )->get_children( ) )
    {
        std::weak_ptr<player> client = std::dynamic_pointer_cast<player>( child );

        if ( _player.lock( ) == client.lock( ) ) continue;

        // 自分の大きさは加味しません。
        if ( distance( _player.lock( )->get_position( ), client.lock( )->get_position( ) )
             < _player.lock( )->get_radius( ) * 0 + client.lock( )->get_radius( ) )
        {
            std::weak_ptr<player> small = _player;
            std::weak_ptr<player> large = client;
            if ( large.lock( )->get_radius( ) < small.lock( )->get_radius( ) )
            {
                swap( small, large );
            }

            if ( small.lock( )->get_radius( ) < large.lock( )->get_radius( ) / 2 )
            {
                if ( !small.lock( )->is_captureing( ) )
                {
                    small.lock( )->captured( large );
                }
            }
        }
    }

    Json::Value root;
    root["name"] = "game_update";
    root["data"]["position"][0] = _player.lock( )->get_position( ).x;
    root["data"]["position"][1] = _player.lock( )->get_position( ).y;
    root["data"]["radius"] = _player.lock( )->get_radius( );

    for ( auto& child : _clients.lock( )->get_children( ) )
    {
        std::weak_ptr<player> client = std::dynamic_pointer_cast<player>( child );
        _udp.lock( )->write( client.lock( )->get_handle( ), root );
    }
}
}