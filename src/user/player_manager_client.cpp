#include "player_manager_client.h"
#include "../network/udp_client.h"
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

    auto client = network::udp_client::create( "127.0.0.1", "25565" );
    _udp = client;
    add_child( client );

    return true;
}
}