#include "player_manager_host.h"
#include "../network/udp_server.h"
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

    auto server = network::udp_server::create( "25565" );
    _udp = server;
    add_child( server );

    return true;
}
}