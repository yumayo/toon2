#include "game.h"
#include "player_manager.h"
#include "create_dot_obeject.h"
#include "scene_manager.h"
#include "title.h"
using namespace cinder;
namespace user
{
CREATE_CPP( game, Json::Value root )
{
    CREATE( game, root );
}
bool game::init( Json::Value& root )
{
    set_schedule_update( );

    if ( auto field = field::create( root ) )
    {
        _field = field;
        add_child( field );
    }

    auto pla_mgr = _field.lock( )->get_child_by_name( "player_manager" );
    _own = std::dynamic_pointer_cast<player>( pla_mgr->get_child_by_name( "own" ) );
    _ground = std::dynamic_pointer_cast<ground>( _field.lock( )->get_child_by_name( "ground" ) );

    if ( auto controller = controller::create( _own, _ground ) )
    {
        _controller = controller;
        add_child( controller );
    }

    auto bac = create_dot_button( "back.png", 64 );
    bac->set_position( vec2( 64 ) + vec2( 10 ) );

    std::string address = user_default::get_instans( )->get_root( )["server"]["address"].asString( );
    int port = user_default::get_instans( )->get_root( )["server"]["port"].asInt( );
    std::weak_ptr<network::udp_connection> udp = std::dynamic_pointer_cast<network::udp_connection>( pla_mgr->get_child_by_name( "udp_connection" ) );
    bac->on_ended = [ address, port, udp ]
    {
        // 一秒ごとにサーバーにデータを送っている方が先に届く可能性がある。
        // TCPと一緒に用いたほうが良さそうですかね。
        // 命令の実行とかは時間かけていい部分ですしね。
        udp.lock( )->set_schedule_update( false );
        udp.lock( )->write( std::make_shared<network::network_object>( address, port ), "{\"name\":\"close\"}" );
        udp.lock( )->remove_from_parent( );
        scene_manager::get_instans( )->replace( title::create( ) );
    };
    add_child( bac );

    return true;
}
void game::update( float delta )
{
    // カメラのアップデート
    auto win_half = vec2( app::getWindowSize( ) ) * 0.5F;
    if ( _own.lock( ) )
        _field.lock( )->set_position( win_half - _own.lock( )->get_position( ) );
}
}