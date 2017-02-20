#pragma once
#include "udp_object.h"
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
namespace network
{
class udp_object::_member
{
public:
    _member( udp_object& parent, std::string const& port );
    _member( udp_object& parent, std::string const & ip_address, std::string const& port );
    ~_member( );
    udp_object& parent;

    asio::io_service io;
    udp::socket socket;

    std::string ip_address;
    std::string port;

    udp::endpoint remote_endpoint_buffer;

    std::unique_ptr<std::thread> recv_thread;

    boost::array<char, 512> buffer;

    char const* class_name = "udp_object";

    void read( udp::endpoint& sender_endpoint );
    void write( std::string const& ip_address, std::string const& port, asio::const_buffers_1 buffer, std::function<void( )> on_send );
    void close( );
};
}