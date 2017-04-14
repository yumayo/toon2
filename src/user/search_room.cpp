#include "search_room.h"
#include "../utility/string_utility.h"
#include "../network/udp_client.h"
#include "../scene_manager.h"
namespace user
{
CREATE_CPP( search_room )
{
    CREATE( search_room );
}
bool search_room::init( )
{
    Json::Value root;
    if ( Json::Reader( ).parse( cinder::app::loadString( "udp.json" ), root ) )
    {
        std::string address, port;
        address = root["server"]["address"].asString( );
        port = root["server"]["port"].asString( );
        auto client = network::udp_client::create( address, port );
        client->set_name( "search_handle" );
        client->write( "{\"name\":\"find_room\"}" );
        client->on_read_failed = [ this ] ( )
        {
            scene_manager::get_instans( )->top( )->set_block_schedule_event( false );
            if ( on_not_found ) on_not_found( );

            remove_from_parent( );
        };
        client->on_readed = [ this, client ] ( const char* str, size_t byte )
        {
            Json::Value host_data;
            std::string data( str, byte );
            if ( Json::Reader( ).parse( data, host_data ) )
            {
                if ( on_founded ) on_founded( host_data );
            }
            else
            {
                scene_manager::get_instans( )->top( )->set_block_schedule_event( false );
                if ( on_not_found ) on_not_found( );
            }
            remove_from_parent( );
        };
        add_child( client );
        return true;
    }
    return false;
}
}