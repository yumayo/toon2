#include "udp_object.h"
#include "udp_object_member.h"
using namespace utility;
namespace network
{
void udp_object::close( )
{
    _m->close( );
}
}