#include "player_manager_client.h"
#include "utility/string_utility.h"
#include "utility/assert_log.h"
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
    if ( !player_manager::init( root ) ) return false;

    auto client = network::udp_connection::create( );
    _udp = client;
    add_child( client );

    return true;
}
}