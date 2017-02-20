#include "player_manager.h"
#include "player.h"
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
    own->set_position( { 200, 0 } );
    own->capture( 30.0F );
    add_child( own );

    auto enemy = player::create( ColorA( 0.6, 0.2, 0.8 ) );
    _enemy = enemy;
    add_child( enemy );

    return true;
}
void player_manager::update( float delta )
{
    get_children( ).sort( [ ] ( std::shared_ptr<node>& a, std::shared_ptr<node>& b )
    {
        std::weak_ptr<player> p_a = std::dynamic_pointer_cast<player>( a );
        std::weak_ptr<player> p_b = std::dynamic_pointer_cast<player>( b );
        return p_a.lock( )->get_radius( ) < p_b.lock( )->get_radius( );
    } );

    std::vector<std::weak_ptr<player>> players;
    for ( auto& child : get_children( ) )
    {
        players.emplace_back( std::dynamic_pointer_cast<player>( child ) );
    }

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
}
std::shared_ptr<player> player_manager::get_player( )
{
    return _player.lock( );
}
}

