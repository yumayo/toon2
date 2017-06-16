#include "player_manager_client.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager_client, Json::Value& root )
{
    CREATE( player_manager_client, root );
}
bool player_manager_client::init( Json::Value& root )
{
    if ( !player_manager::init( root ) ) return false;

    return true;
}
void player_manager_client::update( float delta )
{
    player_manager::update( delta );
}
}