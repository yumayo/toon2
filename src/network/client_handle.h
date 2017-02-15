#pragma once
#include <string>
namespace network
{
class client_handle
{
public:
    client_handle( )
    {

    }
    client_handle( std::string const& name, std::string const& ip_address, int port )
        : _name( name )
        , _ip_address( ip_address )
        , _port( port )
    {

    }
public:
    void clear( );
    void connect( std::string const& ip_address, int port );
    void handshake( std::string const& name );
public:
    friend bool operator==( std::string const&, client_handle const& );
    bool operator==( std::string const& name ) const;
    bool operator==( client_handle const& other ) const;
    bool operator<( client_handle const& other ) const;
    operator bool( )const;
    bool operator!( ) const;
public:
    std::string get_name( )const;
    std::string get_ip_address( )const;
    int get_port( )const;
private:
    std::string _name;
    std::string _ip_address;
    int _port = 0;
};
bool operator==( std::string const& name, client_handle const& other );
}
