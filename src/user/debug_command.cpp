#include "debug_command.h"
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( debug_command, softptr<player> player, softptr<bullet_manager> bullet_manager )
{
    CREATE( debug_command, player, bullet_manager );
}
bool debug_command::init( softptr<player> player, softptr<bullet_manager> bullet_manager )
{
    set_schedule_key_event( );

    _player = player;
    _bullet_manager = bullet_manager;
    on_key_event( app::KeyEvent::KEY_UP, [ this ] ( cinder::app::KeyEvent e )
    {
        _player->capture( 10.0F );
    } );
    on_key_event( app::KeyEvent::KEY_DOWN, [ this ] ( cinder::app::KeyEvent e )
    {
        _player->capture( -10.0F );
    } );
    return true;
}
void debug_command::key_down( cinder::app::KeyEvent e )
{
    auto itr = _key_events.find( e.getCode( ) );
    if ( itr != _key_events.end( ) )
    {
        if ( itr->second ) itr->second( e );
    }
}
void debug_command::on_key_event( int code, std::function<void( cinder::app::KeyEvent )> func )
{
    _key_events.insert( std::make_pair( code, func ) );
}
}
