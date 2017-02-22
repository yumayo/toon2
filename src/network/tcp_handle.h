#pragma once
#define ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
#include "client_handle.h"
#include "boost/array.hpp"
#include <string>
namespace network
{
class tcp_handle
{
public:
    tcp_handle( ) = delete;
    tcp_handle( tcp_handle const& ) = delete;
    tcp_handle( asio::io_service& io );
    void close( );
    void connect( std::string const& ip_address, std::string const& port );
    void buffer_clear( size_t bytes_transferred );
    char const* c_str( );
    asio::ip::tcp::socket socket;
    boost::array<char, 512> buffer;
    client_handle handle;
private: // 以下の値をハンドルに詰め込んで運びます。
    std::string _ip_address;
    std::string _port;
};
}