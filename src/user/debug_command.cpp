#include "debug_command.h"
#include "button_bullet_firing.h"
#include <treelike/action.hpp>
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
    on_key_event( app::KeyEvent::KEY_UP, [ this ]
    {
        _player->capture( 10.0F );
    } );
    on_key_event( app::KeyEvent::KEY_DOWN, [ this ]
    {
        _player->capture( -10.0F );
    } );
    on_key_event( app::KeyEvent::KEY_o, [ this ]
    {
        remove_action_by_name( "debug_key_action" );
        auto act = action::repeat_forever::create( action::sequence::create( action::delay::create( 1.0F ), action::call_func::create( [ this ]
        {
            _player->capture( 10.0F );
        } ) ) );
        act->set_name( "debug_key_action" );
        run_action( act );
    } );
    on_key_event( app::KeyEvent::KEY_p, [ this ]
    {
        remove_action_by_name( "debug_key_action" );
    } );
    return true;
}
void debug_command::key_down( cinder::app::KeyEvent e )
{
    auto itr = _key_events.find( e.getCode( ) );
    if ( itr != _key_events.end( ) )
    {
        if ( itr->second ) itr->second( );
    }
}
void debug_command::on_key_event( int code, std::function<void( )> func )
{
    _key_events.insert( std::make_pair( code, func ) );
}
}
