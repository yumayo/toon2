#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#define ASIO_HAS_STD_ATOMIC

#include "asio/asio.hpp"
#include "boost/array.hpp"

using namespace ci;
using namespace ci::app;

class toon2App : public App {
public:
    toon2App( );
    void send( );
    void recv( );
    void setup( ) override;
    void mouseDown( MouseEvent event ) override;
    void update( ) override;
    void draw( ) override;
    void receive( const asio::error_code&, std::size_t len );
private:
    using udp = asio::ip::udp;
    boost::array<char, 2048> buf;
    asio::io_service receiver;
    asio::io_service sender;
    asio::ip::udp::socket receive_socket;
    asio::ip::udp::socket send_socket;
    CameraPersp camera;
    float angle = 0.0F;
};

toon2App::toon2App( )
    : receive_socket( receiver, asio::ip::udp::endpoint( asio::ip::udp::v4( ), 25565 ) )
    , send_socket( sender )
{
}

void toon2App::send( )
{
    try
    {
        udp::resolver resolver( sender );
        udp::resolver::query query( udp::v4( ), "192.168.11.2", "25565" );
        udp::endpoint receiver_endpoint = *resolver.resolve( query );

        send_socket.open( udp::v4( ) );

        std::string str = "HELLO";
        send_socket.send_to( asio::buffer( str ), receiver_endpoint );
    }
    catch ( std::exception& e )
    {
        std::cerr << e.what( ) << std::endl;
    }
}

void toon2App::recv( )
{
    try
    {
        udp::endpoint remote_endpoint;
        receive_socket.async_receive_from( asio::buffer( buf ), remote_endpoint, std::bind( &toon2App::receive, this, std::placeholders::_1, std::placeholders::_2 ) );
    }
    catch ( std::exception& e )
    {
        std::cerr << e.what( ) << std::endl;
    }
}

void toon2App::setup( )
{
    camera.setPerspective( 60.0F, getWindowAspectRatio( ), 0.1F, 100.0F );
    camera.lookAt( vec3( 0, 0, -5 ), vec3( 0, 0, 0 ) );

    recv( );
}

void toon2App::mouseDown( MouseEvent event )
{
}

void toon2App::update( )
{
    if ( receiver.poll( ) )
    {
        receiver.reset( );
        recv( );
    }

    angle += 0.01F;
}

void toon2App::draw( )
{
    gl::clear( Color( 0, 0, 0 ) );

    gl::enableDepth( );

    gl::setMatrices( camera );

    gl::rotate( angle, vec3( 1, 0, 0 ) );
    gl::rotate( angle, vec3( 0, 1, 0 ) );
    gl::rotate( angle, vec3( 0, 0, 1 ) );

    gl::drawColorCube( vec3( 0, 0, 0 ), vec3( 1, 1, 1 ) );
}

void toon2App::receive( const asio::error_code &, std::size_t len )
{
    console( ).write( buf.data( ), len );
    console( ) << std::endl;

}

CINDER_APP( toon2App, RendererGl, [ & ] ( App::Settings *settings )
{
    settings->setWindowSize( 1280, 720 );
} )