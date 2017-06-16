#include "feed_client.h"
#include "player.h"
#include "feed_manager.h"
#include "scene_manager.h"
#include "network.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( feed_client )
{
    CREATE( feed_client );
}
bool feed_client::init( )
{
    if ( !feed::init( ) ) return false;

    auto dont_destroy_node = scene_manager::get_instans( )->get_dont_destroy_node( );
    _udp_connection = std::dynamic_pointer_cast<network::udp_connection>( dont_destroy_node.lock( )->get_child_by_name( "connection" ) );

    return true;
}
// クライアントは自分自身でエサを作成しません。
// サーバーからの承認を経て作成できます。
void feed_client::on_captured( std::weak_ptr<node> other )
{
    Json::Value root;
    root["name"] = "feed_captured";
    root["data"]["tag"] = get_tag( );
    _udp_connection.lock( )->write( Json::FastWriter( ).write( root ) );

    //auto pla = std::dynamic_pointer_cast<user::player>( other.lock( ) );
    //pla->capture( _score );
    //auto mgr = std::dynamic_pointer_cast<user::feed_manager>( get_parent( ) );
    //mgr->create_feed( );
}
}