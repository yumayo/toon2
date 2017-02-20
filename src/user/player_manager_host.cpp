#include "player_manager_host.h"
#include "../network/udp_client.h"
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

    set_schedule_update( );

    auto server = network::udp_server::create( "25565" );
    _server = server;
    add_child( server );

    return true;
}
void player_manager_host::update( float delta )
{
    vec2 pos = _player.lock( )->get_position( );
    char send_data[sizeof( vec2 )];
    memcpy( send_data, &pos, sizeof( vec2 ) );
    _server.lock( )->write( send_data, sizeof( vec2 ) );

    _server.lock( )->on_readed = [ this ] ( const char* data, size_t size )
    {
        if ( size != sizeof( vec2 ) ) return;
        vec2 pos;
        memcpy( &pos, data, size );
        _enemy.lock( )->set_position( pos );
    };
}
}