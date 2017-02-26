#include "player_manager.h"
#include "player.h"
#include "toon_packet.h"
#include "../user_default.h"
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

    auto& root = user_default::get_instans( )->get_root( );
    auto& select = root["select"];

    if ( select.isNull( ) )
    {
        auto own = player::create( ColorA( 0.2, 0.8, 0.6 ) );
        own->set_name( "own" );
        _player = own;
        add_child( own );
    }
    else
    {
        auto own = player::create( ColorA( 0.2, 0.8, 0.6 ), "skin/" + select.asString( ) + ".png" );
        own->set_name( "own" );
        _player = own;
        add_child( own );
    }

    auto enemy = player::create( ColorA( 0.6, 0.2, 0.8 ) );
    enemy->set_name( "enemy" );
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

    struct player_data
    {
        vec2 position;
        float radius;
    };

    if ( _player.lock( ) )
    {
        player_data p_data = { _player.lock( )->get_position( ), _player.lock( )->get_radius( ) };
        std::unique_ptr<char [ ]> send_data( new char[sizeof( player_data )] );
        memcpy( send_data.get( ), &p_data, sizeof( player_data ) );
        _udp.lock( )->write( send_data.get( ), sizeof( player_data ) );
    }

    _udp.lock( )->on_readed = [ this ] ( const char* data, size_t size )
    {
        if ( _enemy.lock( ) )
        {
            if ( size != sizeof( player_data ) ) return;

            player_data p_data;
            memcpy( &p_data, data, sizeof( player_data ) );
            _enemy.lock( )->set_position( p_data.position );
            _enemy.lock( )->set_radius( p_data.radius );
        }
    };
}
}

