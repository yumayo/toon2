#include "search_room.h"
#include "utility/string_utility.h"
#include "scene_manager.h"
#include <boost/lexical_cast.hpp>
namespace user
{
CREATE_CPP( search_room )
{
    CREATE( search_room );
}
bool search_room::init( )
{
    set_name( "search_room" );
    Json::Value root;
    if ( Json::Reader( ).parse( cinder::app::loadString( "udp.json" ), root ) )
    {
        std::string address;
        int port;
        address = root["server"]["address"].asString( );
        port = boost::lexical_cast<int>( root["server"]["port"].asString( ) );
        auto client = network::udp_connection::create( );
        _client = client;
        add_child( client );
        client->set_name( "connection" );
        client->on_read_failed = [ this ] ( )
        {
            scene_manager::get_instans( )->top( )->set_block_schedule_event( false );
            if ( on_not_found ) on_not_found( );

            remove_from_parent( );
        };
        client->on_received = [ this ] ( network::network_handle handle, const char* str, size_t byte )
        {
            Json::Value received;
            std::string data( str, byte );
            if ( Json::Reader( ).parse( data, received ) )
            {
                if ( on_founded ) on_founded( received );
            }
            else
            {
                scene_manager::get_instans( )->top( )->set_block_schedule_event( false );
                if ( on_not_found ) on_not_found( );
            }
            auto node = scene_manager::get_instans( )->get_dont_destroy_node( );
            _client.lock( )->set_parent( node.lock( ) );
            _client.lock( )->on_received = nullptr;
            _client.lock( )->on_read_failed = nullptr;
        };
        client->write( std::make_shared<network::network_object>( address, port ),
                       "{\"name\":\"find_room\"}" );
        return true;
    }
    return false;
}
}