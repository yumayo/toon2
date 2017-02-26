#include "player_manager_client.h"
#include "../network/udp_client.h"
#include "jsoncpp/json.h"
#include "../utility/string_utility.h"
#include "../utility/assert_log.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager_client )
{
    CREATE( player_manager_client );
}
bool player_manager_client::init( )
{
    if ( !player_manager::init( ) ) return false;

    Json::Value root;
    Json::Reader reader;
    std::string address, port;
    if ( reader.parse( app::loadString( "udp.json" ), root ) )
    {
        address = root["server"]["address"].asString( );
        port = root["server"]["port"].asString( );
    }
    assert_log( !address.empty( ), "無効なアドレスです。", return false );
    assert_log( !port.empty( ), "無効なポートです。", return false );

    auto client = network::udp_client::create( address, port );
    _udp = client;
    add_child( client );

    return true;
}
}