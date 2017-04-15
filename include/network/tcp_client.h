#pragma once
#include "node.h"
#include "asio/error.hpp"
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
    void write( std::string const& message, std::function<void( )> on_send = nullptr );
    void write( char const* message, size_t size, std::function<void( )> on_send = nullptr );
public:
    // データを送れなかったときに呼ばれます。
    std::function<void( )> on_send_failed;
    // データが送られてきたときに呼ばれます。
    std::function<void( char const*, size_t )> on_readed;
    // 接続できなかったときに呼ばれます。
    std::function<void( )> on_connect_failed;
    // 接続が切れたときに呼ばれます。
    std::function<void( )> on_disconnected;
    // ソケットが閉じたときに呼ばれます。
    std::function<void( )> on_closed;
    // その他のエラーが出たときに呼ばれます。
    std::function<void( asio::error_code const& )> on_errored;
};
}