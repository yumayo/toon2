#include "room.h"
#define ASIO_HAS_STD_ATOMIC
#define ASIO_HAS_BOOST_DATE_TIME
#define BOOST_DATE_TIME_NO_LIB
#include "asio/asio.hpp"
using asio::ip::tcp;
using namespace cinder;
namespace meeting
{
struct room::_member
{
    asio::io_service io_service;
    tcp::acceptor acceptor;
    tcp::socket socket;
};
CREATE_CPP( room )
{
    CREATE( room );
}
room::room( )
    : _m( {  } )
{
}
bool room::init( )
{

    return true;
}
}