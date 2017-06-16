#include "player_manager_host.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager_host, Json::Value& root )
{
    CREATE( player_manager_host, root );
}
bool player_manager_host::init( Json::Value& root )
{
    if ( !player_manager::init( root ) ) return false;

    return true;
}
void player_manager_host::update( float delta )
{
    player_manager::update( delta );
}
}