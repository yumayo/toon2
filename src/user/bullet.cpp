#include "bullet.h"
#include <treelike/utility.hpp>
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( bullet, int bullet_id, float radius, cinder::ColorA color )
{
    CREATE( bullet, bullet_id, radius, color );
}
bool bullet::init( int bullet_id, float radius, cinder::ColorA color )
{
    utility::log( "bullet[%d]", bullet_id );
    _radius = radius;
    set_tag( bullet_id );
    set_color( color );
    set_content_size( vec2( radius * 2 ) );
    set_anchor_point( vec2( 0.5F ) );
    set_pivot( vec2( 0.5F ) );
    return true;
}
float bullet::get_radius( ) const
{
    return _radius;
}
void bullet::hit( )
{
    _is_hit = true;
}
bool bullet::is_hit( )
{
    return _is_hit;
}
}
