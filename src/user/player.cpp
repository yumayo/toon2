#include "player.h"
using namespace cinder;
namespace user
{
CREATE_CPP( player )
{
    CREATE( player );
}
bool player::init( )
{
    set_name( "player" );
    set_color( ColorA( 0.2, 0.8, 0.6 ) );
    set_radius( 20.0F );

    if ( auto base = renderer::circle::create( _radius ) )
    {
        _base = base;
        add_child( base );

        if ( auto mask = renderer::circle::create( _radius ) )
        {
            _mask = mask;
            mask->set_color( get_color( ) );
            mask->set_scale( vec2( 0.9F ) );
            base->add_child( mask );
        }
    }

    return true;
}
void player::set_radius( float value )
{
    _radius = value;
}
float player::get_radius( )
{
    return _radius;
}
void player::predation( float score )
{
    set_radius( get_radius( ) + score );
    _base.lock( )->set_radius( get_radius( ) );
    _mask.lock( )->set_radius( get_radius( ) );
}
}

