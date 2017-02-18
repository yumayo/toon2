#include "field.h"
#include "feed.h"
#include "player.h"
#include "cinder/Rand.h"
using namespace cinder;
namespace user
{
CREATE_CPP( field )
{
    CREATE( field );
}
bool field::init( )
{
    if ( !renderer::surface::init( vec2( 512 ) ) ) return false;

    set_schedule_update( );

    auto feed_manager = node::create( );
    add_child( feed_manager );
    _feed_manager = feed_manager;
    for ( int i = 0; i < 10; ++i )
    {
        if ( auto f = feed::create( ) )
        {
            f->set_position( vec2( randFloat( _content_size.x ), randFloat( _content_size.y ) ) );
            feed_manager->add_child( f );
        }
    }

    return true;
}
void field::update( float delta )
{
    auto player = get_child_by_name( "player" );
    feed_collide_update( player );
    player_paint( player );
}
void field::feed_collide_update( std::weak_ptr<node> player )
{
    auto pl = static_cast<user::player*>( player.lock( ).get( ) );
    for ( auto& f : _feed_manager.lock( )->get_children( ) )
    {
        auto fe = static_cast<user::feed*>( f.get( ) );

        auto pos = pl->get_position( );
        auto radius = pl->get_radius( );
        auto target_pos = fe->get_position( );

        if ( distance( pos, target_pos ) < radius )
        {
            pl->predation( fe->capture( ) );
        }
    }
}
void field::player_paint( std::weak_ptr<node> player )
{
    auto pl = static_cast<user::player*>( player.lock( ).get( ) );

    float radius = pl->get_radius( );
    for ( int y = -radius; y < radius; ++y )
    {
        for ( int x = -radius; x < radius; ++x )
        {
            auto len = length( vec2( x, y ) );
            if ( len < radius )
            {
                set_pixel( player.lock( )->get_position( ) + vec2( x, y ), player.lock( )->get_color( ) );
            }
        }
    }
}
}

