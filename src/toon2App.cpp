#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#define ASIO_HAS_STD_ATOMIC

#define ASIO_HAS_BOOST_DATE_TIME

#define BOOST_DATE_TIME_NO_LIB

#include "asio/asio.hpp"
#include "boost/array.hpp"

#include <string.h>
#include <thread>

using namespace ci;
using namespace ci::app;

class recv_obejct
{
public:
    recv_obejct( asio::io_service::work** work, std::function<void( )> const& function );
    ~recv_obejct( );
private:
    std::shared_ptr<std::thread> thread;
};

class toon2App : public App {
public:
    toon2App( );
    void send( );
    void recv( );
    void setup( ) override;
    void cleanup( ) override;
    void mouseDown( MouseEvent event ) override;
    void touchesBegan( TouchEvent event ) override;
    void touchesMoved( TouchEvent event ) override;
    void touchesEnded( TouchEvent event ) override;
    void update( ) override;
    void draw( ) override;
    void receive( const asio::error_code& error, std::size_t len );
private:
    using udp = asio::ip::udp;
    boost::array<char, 2048> buf;
    asio::io_service sender;
    asio::ip::udp::socket send_socket;
    asio::io_service receiver;
    asio::io_service::work* work;
    asio::ip::udp::socket receive_socket;
    CameraPersp camera;
    float angle = 0.0F;
    vec2 pos;
    bool polling = false;
    recv_obejct r_object;
};

toon2App::toon2App( )
    : send_socket( sender )
    , receive_socket( receiver, asio::ip::udp::endpoint( asio::ip::udp::v4( ), 25565 ) )
    , r_object( &work, std::bind( &toon2App::recv, this ) )
{
    work = new asio::io_service::work( receiver );
    buf.fill( 0 );
}

void toon2App::send( )
{
    try
    {
        udp::resolver resolver( sender );
        udp::resolver::query query( udp::v4( ), "192.168.11.2", "25565" );
        udp::endpoint receiver_endpoint = *resolver.resolve( query );

        send_socket.open( udp::v4( ) );

        char str[sizeof( pos )];
        memcpy( str, &pos, sizeof( pos ) );
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
        receive_socket.async_receive_from( asio::buffer( buf ), remote_endpoint, [ this ] ( const asio::error_code & error, std::size_t len )
        {
            memcpy( &pos, buf.data( ), sizeof( pos ) );
        } );
        receiver.reset( );
        receiver.run( );
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
}

void toon2App::cleanup( )
{
    delete work;
    work = nullptr;

    receiver.stop( );
}

void toon2App::mouseDown( MouseEvent event )
{
}

void toon2App::touchesBegan( TouchEvent event )
{
    for ( auto& touch : event.getTouches( ) )
    {
        pos = touch.getPos( );
    }
}

void toon2App::touchesMoved( TouchEvent event )
{
    for ( auto& touch : event.getTouches( ) )
    {
        pos = touch.getPos( );
    }
}

void toon2App::touchesEnded( TouchEvent event )
{
}

void toon2App::update( )
{
    angle += 0.01F;
}

void toon2App::draw( )
{
    gl::clear( Color( 0, 0, 0 ) );

    gl::enableDepth( true );
    gl::setMatrices( camera );
    gl::rotate( angle, vec3( 1, 0, 0 ) );
    gl::rotate( angle, vec3( 0, 1, 0 ) );
    gl::rotate( angle, vec3( 0, 0, 1 ) );
    gl::drawColorCube( vec3( 0, 0, 0 ), vec3( 1, 1, 1 ) );

    gl::enableDepth( false );
    gl::setMatricesWindow( getWindowSize( ) );
    gl::drawSolidCircle( pos, 10 );
}

void toon2App::receive( const asio::error_code & error, std::size_t len )
{
}

CINDER_APP( toon2App, RendererGl, [ & ] ( App::Settings *settings )
{
    settings->setWindowSize( 1280, 720 );
} )

recv_obejct::recv_obejct( asio::io_service::work** work, std::function<void( )> const& function )
{
    thread = std::make_shared<std::thread>( [ this, function, work ]
    {
        while ( *work )
        {
            function( );
        }
    } );
}

recv_obejct::~recv_obejct( )
{
    thread->join( );
}
