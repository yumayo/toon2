#include "game.h"
using namespace cinder;
namespace user
{
CREATE_CPP( game )
{
    CREATE( game );
}
bool game::init( )
{
    set_schedule_update( );

    if ( auto field = field::create( ) )
    {
        _field = field;
        add_child( field );
    }

    // プレイヤーを探します。
    auto pla_mgr = _field.lock( )->get_child_by_name( "player_manager" );
    auto own = pla_mgr->get_child_by_name( "own" );
    _own = std::dynamic_pointer_cast<player>( own );

    if ( auto controller = controller::create( _own ) )
    {
        _controller = controller;
        add_child( controller );
    }
    return true;
}
void game::update( float delta )
{
    // カメラのアップデート
    auto win_half = vec2( app::getWindowSize( ) ) * 0.5F;
    _field.lock( )->set_position( win_half - _own.lock( )->get_position( ) );
}
}