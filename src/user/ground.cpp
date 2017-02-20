#include "ground.h"
#include "player.h"
using namespace cinder;
namespace user
{
CREATE_CPP( ground, std::weak_ptr<node> player_manager )
{
    CREATE( ground, player_manager );
}
bool ground::init( std::weak_ptr<node> player_manager )
{
    if ( !renderer::surface::init( vec2( 512 ), ColorA( 0.1F, 0.1F, 0.1F ) ) ) return false;

    _player_manager = player_manager;

    set_schedule_update( );

    set_scale( vec2( 4.0F ) );

    return true;
}
void ground::update( float delta )
{
    auto own = _player_manager.lock( )->get_child_by_name( "own" );
    auto pl = std::dynamic_pointer_cast<player>( own );

    float radius = pl->get_radius( ) / get_scale( ).x;
    for ( int y = -radius; y < radius; ++y )
    {
        for ( int x = -radius; x < radius; ++x )
        {
            auto len = length( vec2( x, y ) );
            if ( len < radius )
            {
                set_pixel( pl->get_position( ) / get_scale( ).x + vec2( x, y ), pl->get_color( ) );
            }
        }
    }

    for ( auto& player : _player_manager.lock( )->get_children( ) )
    {
        auto pos = player->get_position( );
        auto size = get_content_size( ) * get_scale( );

        pos.x = clamp( pos.x, 0.0F, size.x );
        pos.y = clamp( pos.y, 0.0F, size.y );
         
        player->set_position( pos );
    }
}
}

