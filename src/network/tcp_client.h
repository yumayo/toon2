#pragma once
#include "../node.h"
namespace network
{
class tcp_client : public node
{
    struct _member;
    std::shared_ptr<_member> _m;
public:
    CREATE_H( tcp_client, std::string const& ip_address, std::string const& port );
    bool init( std::string const& ip_address, std::string const& port );
    ~tcp_client( );
    void update( float delta ) override;
public:
    void write( std::string const& message, std::function<void(bool)> on_writed_call = nullptr );
    void write( char const* message, size_t size, std::function<void( bool )> on_writed_call = nullptr );
private:
    // Luaにバインドする時に使います。
    void write_string( std::string const& message, std::function<void( bool )> on_writed_call );
    // Luaにバインドする時に使います。
    void write_binary( char const* message, size_t size, std::function<void( bool )> on_writed_call );
public:
    std::function<void( std::string const& )> on_readed_call;
public:
    LUA_SETUP_H( tcp_client );
};
}