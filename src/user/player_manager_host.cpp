#include "player_manager_host.h"
#include "network/udp_connection.h"
#include "jsoncpp/json.h"
#include "utility/string_utility.h"
#include "utility/assert_log.h"
#include "boost/lexical_cast.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager_host, Json::Value& root, std::shared_ptr<network::udp_connection>& connection )
{
    CREATE( player_manager_host, root, connection );
}
bool player_manager_host::init( Json::Value& root, std::shared_ptr<network::udp_connection>& connection )
{
    if ( !player_manager::init( root ) ) return false;

    _udp = connection;
    connection->set_parent( shared_from_this( ) );

    return true;
}
}