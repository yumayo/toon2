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

    return true;
}
}