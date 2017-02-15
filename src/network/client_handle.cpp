#include "client_handle.h"
namespace network
{
void client_handle::clear( )
{
    _name = "";
    _ip_address = "";
    _port = 0;
}
void client_handle::connect( std::string const & ip_address, int port )
{
    _ip_address = ip_address;
    _port = port;
}
void client_handle::handshake( std::string const & name )
{
    _name = name;
}
bool client_handle::operator==( std::string const & name ) const
{
    return this->_name == name;
}
bool client_handle::operator==( client_handle const & other ) const
{
    return this->_name == other._name;
}
bool client_handle::operator<( client_handle const & other ) const
{
    return this->_name < other._name;
}
client_handle::operator bool( ) const
{
    return !_name.empty( );
}
bool client_handle::operator!( ) const
{
    return _name.empty( );
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
bool operator==( std::string const & name, client_handle const& other )
{
    return name == other._name;
}
}
