#include "meeting_room.h"
using namespace cinder;
namespace user
{
CREATE_CPP( meeting_room )
{
    CREATE( meeting_room );
}
bool meeting_room::init( )
{
    return true;
}
}