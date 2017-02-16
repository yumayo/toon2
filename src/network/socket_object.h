#pragma once
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using asio::ip::tcp;
#include "client_handle.h"
#include "boost/array.hpp"
namespace network
{
struct socket_object
{
    socket_object( asio::io_service& io );
    void close( );
    void buffer_clear( size_t bytes_transferred );
    void handshake( );
    char const* c_str( );
    tcp::socket socket;
    client_handle handle;
    boost::array<char, 512> buffer;
};
}
