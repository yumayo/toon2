#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#include "cinder/ImageIo.h"

#define ASIO_HAS_STD_ATOMIC

#define ASIO_HAS_BOOST_DATE_TIME

#define BOOST_DATE_TIME_NO_LIB

#include "asio/asio.hpp"
#include "boost/array.hpp"

#include <string.h>
#include <thread>

using namespace ci;
using namespace ci::app;


int my_port = 51264;
int other_port = 25566;
std::string ip = "127.0.0.1";

class recv_obejct
{
public:
    recv_obejct( asio::io_service& receiver, std::function<void( )> const& function );
    ~recv_obejct( );
private:
    asio::io_service& receiver;
    bool stop = false;
    std::shared_ptr<std::thread> thread;
};

class toon2App : public App
{
public:
    toon2App( );
    void send( );
    void recv( );
    void setup( ) override;
    void cleanup( ) override;
    void mouseDown( MouseEvent event ) override {}
    void mouseUp( MouseEvent event ) override {}
    void mouseWheel( MouseEvent event ) override {}
    void mouseMove( MouseEvent event ) override {}
    void mouseDrag( MouseEvent event ) override
    {
        myPos = event.getPos( );
        memcpy( myBuf.data( ), &myPos, sizeof( myPos ) );
    }
    void touchesBegan( TouchEvent event ) override { }
    void touchesMoved( TouchEvent event ) override { }
    void touchesEnded( TouchEvent event ) override { }
    void update( ) override;
    void draw( ) override;
private:
    using udp = asio::ip::udp;
    boost::array<char, 2048> myBuf;
    boost::array<char, 2048> otherBuf;
    asio::io_service sender;
    asio::ip::udp::socket send_socket;
    CameraPersp camera;
    float angle = 0.0F;
    vec2 myPos = vec2( 0, 0 );
    vec2 otherPos = vec2( 0, 0 );
    asio::io_service receiver;
    asio::ip::udp::socket receive_socket;
    recv_obejct r_object;
};

toon2App::toon2App( )
    : send_socket( sender )
    , receive_socket( receiver, asio::ip::udp::endpoint( asio::ip::udp::v4( ), my_port ) )
    , r_object( receiver, std::bind( &toon2App::recv, this ) )
{
    myBuf.fill( 0 );
    otherBuf.fill( 0 );

    //send_socket.open( udp::v4( ) );
}

void toon2App::send( )
{
    try
    {
        udp::resolver resolver( sender );
        udp::resolver::query query( udp::v4( ), ip, std::to_string( other_port ) );
        udp::endpoint receiver_endpoint = *resolver.resolve( query );

        send_socket.send_to( asio::buffer( myBuf ), receiver_endpoint );
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
        receive_socket.async_receive_from( asio::buffer( otherBuf ), remote_endpoint, [ this ] ( const asio::error_code & error, std::size_t len )
        {
            memcpy( &otherPos, otherBuf.data( ), sizeof( otherPos ) );
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
    send_socket.close( );
}

void toon2App::update( )
{
    send( );
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
    gl::color( Color( 0, 1, 0 ) );
    gl::drawSolidCircle( myPos, 10 );
    gl::color( Color( 1, 0, 0 ) );
    gl::drawSolidCircle( otherPos, 10 );
    gl::color( Color( 1, 1, 1 ) );
}


recv_obejct::recv_obejct( asio::io_service& receiver, std::function<void( )> const& function )
    : receiver( receiver )
{
    thread = std::make_shared<std::thread>( [ this, function ]
    {
        while ( !stop )
        {
            function( );
        }
    } );
}

recv_obejct::~recv_obejct( )
{
    receiver.stop( );
    stop = true;
    thread->join( );
}
