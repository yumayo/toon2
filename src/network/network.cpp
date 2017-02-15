#include "network.hpp"
#include "asio/error.hpp"
#include "../utility/string_utility.h"
using namespace utility;
namespace network
{
#define l_e(__name) #__name, asio::error::__name
void lua_setup_asio_error( sol::state& lua )
{
    lua.new_enum( "asio_error"
                  // basic_errors 
                  , l_e( access_denied )
                  , l_e( address_family_not_supported )
                  , l_e( address_in_use )
                  , l_e( already_connected )
                  , l_e( already_started )
                  , l_e( broken_pipe )
                  , l_e( connection_aborted )
                  , l_e( connection_refused )
                  , l_e( connection_reset )
                  , l_e( bad_descriptor )
                  , l_e( fault )
                  , l_e( host_unreachable )
                  , l_e( in_progress )
                  , l_e( interrupted )
                  , l_e( invalid_argument )
                  , l_e( message_size )
                  , l_e( name_too_long )
                  , l_e( network_down )
                  , l_e( network_reset )
                  , l_e( network_unreachable )
                  , l_e( no_descriptors )
                  , l_e( no_buffer_space )
                  , l_e( no_memory )
                  , l_e( no_permission )
                  , l_e( no_protocol_option )
                  , l_e( no_such_device )
                  , l_e( not_connected )
                  , l_e( not_socket )
                  , l_e( operation_aborted )
                  , l_e( operation_not_supported )
                  , l_e( shut_down )
                  , l_e( timed_out )
                  , l_e( try_again )
                  , l_e( would_block )

                  // netdb_errors
                  , l_e( host_not_found )
                  , l_e( host_not_found_try_again )
                  , l_e( no_data )
                  , l_e( no_recovery )

                  // addrinfo_errors
                  , l_e( service_not_found )
                  , l_e( socket_type_not_supported )

                  // misc_errors
                  , l_e( already_open )
                  , l_e( eof )
                  , l_e( not_found )
                  , l_e( fd_set_failure )
    );
}
#undef l_e
void lua_setup_client_handle( sol::state& lua )
{
    
}
void log_data( char const * data, size_t bytes_transferred )
{
    std::string output = ">[ ";
    size_t i;
    for ( i = 0; i < bytes_transferred - 1; ++i )
    {
        if ( ( i % 16 ) == 15 )
        {
            output += format( "%02X", data[i] & 0x000000FF );
            output += " ]\n>[ ";
        }
        else
        {
            if ( ( i % 4 ) == 3 )
            {
                output += format( "%02X | ", data[i] & 0x000000FF );
            }
            else
            {
                output += format( "%02X", data[i] & 0x000000FF );
            }
        }
    }
    output += format( "%02X", data[i] & 0x000000FF );
    log( "%s ]", output.c_str( ) );
}
void lua_setup( sol::state& lua )
{
    lua_setup_asio_error( lua );
    lua_setup_client_handle( lua );
    tcp_server::lua_setup( lua );
    tcp_client::lua_setup( lua );
}
}
