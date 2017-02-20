#include "enemy.h"
#include "../network/udp_reader.h"
using namespace cinder;
namespace user
{
CREATE_CPP( enemy )
{
    CREATE( enemy );
}
bool enemy::init( )
{
    if ( !player::init( ColorA( 0.6, 0.2, 0.8 ) ) ) return false;

    set_name( "enemy" );

    return true;
}
}