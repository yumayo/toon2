#include "feed_manager.h"
#include "ground.h"
#include "feed.h"
#include "player.h"
#include "cinder/Rand.h"
using namespace cinder;
namespace user
{
CREATE_CPP( feed_manager, std::weak_ptr<node> player_manager, std::weak_ptr<node> ground )
{
    CREATE( feed_manager, player_manager, ground );
}
bool feed_manager::init( std::weak_ptr<node> player_manager, std::weak_ptr<node> ground )
{
    set_name( "feed_manager" );

    _player_manager = player_manager;

    set_schedule_update( );

    auto size = ground.lock( )->get_content_size( );

    // エサの作成。
    for ( int i = 0; i < 10; ++i )
    {
        if ( auto f = feed::create( ) )
        {
            f->set_position( vec2( randFloat( size.x ), randFloat( size.y ) ) );
            add_child( f );
        }
    }

    return true;
}
void feed_manager::update( float delta )
{
    auto own = _player_manager.lock( )->get_child_by_name( "own" );
    auto pla = std::dynamic_pointer_cast<player>( own );

    for ( auto& f : _children )
    {
        auto fee = std::dynamic_pointer_cast<feed>( f );

        auto pos = pla->get_position( );
        auto radius = pla->get_radius( );
        auto target_pos = fee->get_position( );

        if ( distance( pos, target_pos ) < radius )
        {
            fee->captured( pla );
        }
    }
}
}

