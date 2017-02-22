#pragma once
#include "udp_server.h"
#ifndef ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_STD_ATOMIC
#endif
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using udp = asio::ip::udp;
#include "boost/array.hpp"
#include "../utility/string_utility.h"
#include "boost/lexical_cast.hpp"
#include "client_handle.h"
using namespace utility;
namespace network
{
class udp_server::_member
{
public:
    _member( udp_server& parent, std::string const& port );
    ~_member( );

    udp_server& parent;

    asio::io_service io;

    std::string server_port;

    udp::endpoint remote_endpoint_buffer;

    udp::socket socket;

    boost::array<char, 512> buffer;

    char const* class_name = "udp_server";

    void update( );
    void read( );
    void write( asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void close( );
};
}