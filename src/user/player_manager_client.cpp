#include "player_manager_client.h"
#include "../network/udp_client.h"
#include "../utility/string_utility.h"
#include "../utility/assert_log.h"
#include "boost/lexical_cast.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager_client, Json::Value& root )
{
    CREATE( player_manager_client, root );
}
bool player_manager_client::init( Json::Value& root )
{
    if ( !player_manager::init( ) ) return false;

    std::string ip_address = root["DATA"]["ip_address"].asString( );
    // udp_objectを使いまわせていないのでportに +1 してしのいでいます。
    std::string port = boost::lexical_cast<std::string>( root["DATA"]["port"].asInt( ) + 1 );

    auto client = network::udp_client::create( ip_address, port );

    auto col = _player.lock( )->get_color( );
    _player.lock( )->set_color( _enemy.lock( )->get_color( ) );
    _enemy.lock( )->set_color( col );

    _udp = client;
    add_child( client );

    return true;
}
}