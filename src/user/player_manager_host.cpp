#include "player_manager_host.h"
#include "../network/udp_server.h"
#include "jsoncpp/json.h"
#include "../utility/string_utility.h"
#include "../utility/assert_log.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager_host )
{
    CREATE( player_manager_host );
}
bool player_manager_host::init( )
{
    if ( !player_manager::init( ) ) return false;

    Json::Value root;
    Json::Reader reader;
    std::string port;
    if ( reader.parse( app::loadString( "udp.json" ), root ) )
    {
        port = root["server"]["port"].asString( );
    }
    assert_log( !port.empty( ), "無効なポートです。", return false );

    auto server = network::udp_server::create( port );
    _udp = server;
    add_child( server );

    return true;
}
}