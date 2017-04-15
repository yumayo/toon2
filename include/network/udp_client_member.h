#pragma once
#include "udp_client.h"
#ifndef ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_STD_ATOMIC
#endif
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using udp = asio::ip::udp;
#include "boost/array.hpp"
#include "utility/string_utility.h"
#include "boost/lexical_cast.hpp"
using namespace utility;
namespace network
{
class udp_client::_member
{
public:
    _member( udp_client& parent, std::string const & ip_address, std::string const& port );
    ~_member( );

    udp_client& parent;

    asio::io_service io;
    udp::socket socket;

    std::string ip_address;
    std::string port;

    udp::endpoint remote_endpoint_buffer;

    boost::array<char, 512> buffer;

    char const* class_name = "udp_client";

    void update( );
    void read( );
    void write( asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void close( );
};
}