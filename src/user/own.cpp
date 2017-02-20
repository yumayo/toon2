#include "own.h"
#include "../network/udp_sender.h"
using namespace cinder;
namespace user
{
CREATE_CPP( own )
{
    CREATE( own );
}
bool own::init( )
{
    if ( !player::init( ColorA( 0.2, 0.8, 0.6 ) ) ) return false;

    set_name( "own" );

    return true;
}
}