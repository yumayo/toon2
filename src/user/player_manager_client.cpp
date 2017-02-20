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

    set_schedule_update( );

    auto client = network::udp_client::create( "192.168.11.2", "25565" );
    _client = client;
    add_child( client );

    return true;
}
void player_manager_client::update( float delta )
{
    vec2 pos = _player.lock( )->get_position( );
    char send_data[sizeof( vec2 )];
    memcpy( send_data, &pos, sizeof( vec2 ) );
    _client.lock( )->write( send_data, sizeof( vec2 ) );

    _client.lock( )->on_readed = [ this ] ( const char* data, size_t size )
    {
        vec2 pos;
        memcpy( &pos, data, size );
        _enemy.lock( )->set_position( pos );
    };
}
}