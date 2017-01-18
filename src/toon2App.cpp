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
    recv_obejct( std::function<void( )> const& function );
    ~recv_obejct( );
private:
    bool kill = false;

    std::shared_ptr<std::thread> thread;
};

class toon2App : public App {
public:
    toon2App( );
    void send( );
    void recv( );
    void setup( ) override;
    void mouseDown( MouseEvent event ) override;
    void update( ) override;
    void draw( ) override;
    void receive( const asio::error_code& error, std::size_t len );
private:
    using udp = asio::ip::udp;
    boost::array<char, 2048> buf;
    asio::io_service sender;
    asio::ip::udp::socket send_socket;
    CameraPersp camera;
    float angle = 0.0F;
    vec2 pos;
    bool polling = false;
    recv_obejct recever;
};

toon2App::toon2App( )
    : send_socket( sender )
    , recever( std::bind( &toon2App::recv, this ) )
{
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

        std::string str = "HELLO";
        send_socket.send_to( asio::buffer( str ), receiver_endpoint );
    }
    catch ( std::exception& e )
    {
        std::cerr << e.what( ) << std::endl;
    }
}

#include <ctime>

void toon2App::recv( )
{
    try
    {
        asio::io_service receiver;
        asio::deadline_timer connect_timer( receiver );
        asio::ip::udp::socket receive_socket( receiver, asio::ip::udp::endpoint( asio::ip::udp::v4( ), 25565 ) );
        udp::endpoint remote_endpoint;
        receive_socket.async_receive_from( asio::buffer( buf ), remote_endpoint, [ this, &connect_timer ] ( const asio::error_code & error, std::size_t len )
        {
            connect_timer.cancel( );

            console( ).write( buf.data( ), len + 1 );
            console( ) << std::endl;
            console( ) << std::endl;
        } );

        // 5秒でタイムアウト
        connect_timer.expires_from_now( boost::posix_time::seconds( 5 ) );
        connect_timer.async_wait( [ &receive_socket ] ( const asio::error_code& error )
        {
            if ( !error )
            {
                // タイムアウト : 接続を切る。接続のハンドラがエラーになる
                receive_socket.close( );
            }
        } );
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

void toon2App::mouseDown( MouseEvent event )
{
}

void toon2App::update( )
{
    memcpy( &pos, buf.data( ), sizeof( pos ) );

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

CINDER_APP( toon2App, RendererGl, [ & ] ( App::Settings *settings )
{
    settings->setWindowSize( 1280, 720 );
} )

recv_obejct::recv_obejct( std::function<void( )> const& function )
{
    thread = std::make_shared<std::thread>( [ this, function ]
    {
        while ( !kill )
        {
            function( );
        }
    } );
}

recv_obejct::~recv_obejct( )
{
    kill = true;
    thread->join( );
}
