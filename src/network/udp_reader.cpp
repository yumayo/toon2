#include "udp_reader.h"
#define ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using udp = asio::ip::udp;
#include "boost/array.hpp"
#include "../utility/string_utility.h"
#include "boost/lexical_cast.hpp"
using namespace utility;
namespace network
{
struct udp_reader::_member
{
    _member( udp_reader& parent, std::string port );
    ~_member( );
    udp_reader& parent;
    asio::io_service io;
    udp::socket socket;
    boost::array<char, 512> buffer;
    udp::endpoint remote_endpoint;
    std::string port;
    void read( );
    void close( );
};
CREATE_CPP( udp_reader, std::string const& port )
{
    CREATE( udp_reader, port );
}
bool udp_reader::init( std::string const& port )
{
    _m = std::make_shared<_member>( *this, port );

    set_schedule_update( );

    return true;
}
void udp_reader::update( float delta )
{
    _m->io.reset( );
    _m->io.poll( );
}
void udp_reader::close( )
{
    _m->close( );
}
udp_reader::_member::_member( udp_reader& parent, std::string port )
    : parent( parent )
    , socket( io, asio::ip::udp::endpoint( asio::ip::udp::v4( ), boost::lexical_cast<int>( port ) ) )
    , port( port )
{
    read( );
}
udp_reader::_member::~_member( )
{
    close( );
}
void udp_reader::_member::read( )
{
    socket.async_receive_from( asio::buffer( buffer ),
                               remote_endpoint,
                               [ this ] ( const asio::error_code& error, size_t bytes_transferred )
    {
        if ( error )
        {
            log( "【udp_reader】データを受け取れませんでした。" );
            if ( parent.on_read_failed ) parent.on_read_failed( );
        }
        else
        {
            log( "【udp_reader】受け取ったデータ: %d byte", bytes_transferred );
            log_data( buffer.data( ), bytes_transferred );
            if ( parent.on_readed ) parent.on_readed( buffer.data( ), bytes_transferred );
            std::fill_n( buffer.begin( ), bytes_transferred, 0 );

            // エラーじゃない限り無限に受け取りを続けます。
            read( );
        }
    } );
}
void udp_reader::_member::close( )
{
    if ( parent.on_closed ) parent.on_closed( );
    socket.close( );
}
}