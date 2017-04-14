#include "player_manager_host.h"
#include "../network/udp_server.h"
#include "jsoncpp/json.h"
#include "../utility/string_utility.h"
#include "../utility/assert_log.h"
#include "boost/lexical_cast.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager_host, Json::Value& root )
{
    CREATE( player_manager_host, root );
}
bool player_manager_host::init( Json::Value& root )
{
    if ( !player_manager::init( ) ) return false;

    std::string ip_address = root["data"]["ip_address"].asString( );
    // udp_objectを使いまわせていないのでportに +1 してしのいでいます。
    std::string port = boost::lexical_cast<std::string>( root["data"]["port"].asInt( ) + 1 );

    auto server = network::udp_server::create( port );
    _udp = server;
    add_child( server );

    return true;
}
}