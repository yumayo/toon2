#include "client_handle.h"
namespace network
{
client_handle::client_handle( std::string const & name, std::string const & ip_address, int const & port )
    : _name( name )
    , _ip_address( ip_address )
    , _port( port )
{

}
bool client_handle::operator==( std::string const & name ) const
{
    return this->_name == name;
}
bool client_handle::operator==( client_handle const & other ) const
{
    return this->_name == other._name;
}
std::string client_handle::get_name( ) const
{
    return _name;
}
std::string client_handle::get_ip_address( ) const
{
    return _ip_address;
}
int client_handle::get_port( ) const
{
    return _port;
}
#define l_class client_handle
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( client_handle
           , l_readonly( name )
           , l_readonly( ip_address )
           , l_readonly( port )
    );
}
#include "lua_undef.h"
bool operator==( std::string const & name, client_handle const& other )
{
    return name == other._name;
}
}
