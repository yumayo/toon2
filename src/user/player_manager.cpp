#include "player_manager.h"
#include "player.h"
#include "toon_packet.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player_manager )
{
    CREATE( player_manager );
}
bool player_manager::init( )
{
    set_name( "player_manager" );

    set_schedule_update( );

    auto own = player::create( ColorA( 0.2, 0.8, 0.6 ) );
    _player = own;
    own->set_position( { 800, 0 } );
    own->capture( 30.0F );
    add_child( own );

    auto enemy = player::create( ColorA( 0.6, 0.2, 0.8 ) );
    _enemy = enemy;
    add_child( enemy );

    return true;
}
void player_manager::update( float delta )
{
    // 大きさで、描画順を変更。
    get_children( ).sort( [ ] ( std::shared_ptr<node>& a, std::shared_ptr<node>& b )
    {
        std::weak_ptr<player> p_a = std::dynamic_pointer_cast<player>( a );
        std::weak_ptr<player> p_b = std::dynamic_pointer_cast<player>( b );
        if ( p_a.lock( ) && p_b.lock( ) )
        {
            return p_a.lock( )->get_radius( ) < p_b.lock( )->get_radius( );
        }
        return false;
    } );

    // プレイヤーオブジェクトを詰め込みます。
    // サーバーが含まれているので、ダイキャスト時にnullチェック。
    std::vector<std::weak_ptr<player>> players;
    for ( auto& child : get_children( ) )
    {
        if ( auto pla = std::dynamic_pointer_cast<player>( child ) )
        {
            players.emplace_back( pla );
        }
    }

    // プレイヤー同士の当たり判定。
    for ( auto& child : players )
    {
        for ( auto& other : players )
        {
            if ( child.lock( ) == other.lock( ) ) continue;

            // 自分の大きさは加味しません。
            if ( distance( child.lock( )->get_position( ), other.lock( )->get_position( ) )
                 < child.lock( )->get_radius( ) * 0 + other.lock( )->get_radius( ) )
            {
                std::weak_ptr<player> small = child;
                std::weak_ptr<player> large = other;
                if ( large.lock( )->get_radius( ) < small.lock( )->get_radius( ) )
                {
                    swap( small, large );
                }

                if ( small.lock( )->get_radius( ) < large.lock( )->get_radius( ) / 2 )
                {
                    if ( !small.lock( )->is_captureing( ) ) small.lock( )->captured( large );
                }
            }
        }
    }

    // 相手に自分のデータを送ります。
    toon_data t_data;
    t_data.position = _player.lock( )->get_position( );
    t_data.radius = _player.lock( )->get_radius( );
    char send_data[sizeof( toon_data )];
    memcpy( send_data, &t_data, sizeof( toon_data ) );
    _udp.lock( )->write( send_data, sizeof( toon_data ) );

    _udp.lock( )->on_readed = [ this ] ( const char* data, size_t size )
    {
        if ( _enemy.lock( ) )
        {
            if ( size != sizeof( toon_data ) ) return;
            toon_data t_data;
            memcpy( &t_data, data, size );
            _enemy.lock( )->set_position( t_data.position );
            _enemy.lock( )->set_radius( t_data.radius );
        }
    };
}
std::shared_ptr<player> player_manager::get_player( )
{
    return _player.lock( );
}
}

