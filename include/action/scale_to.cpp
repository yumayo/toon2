#include "scale_to.h"
#include "node.h"
using namespace cinder;
namespace action
{
CREATE_CPP( scale_to, float duration, cinder::vec2 scale )
{
    CREATE( scale_to, duration, scale );
}
bool scale_to::init( float duration, cinder::vec2 scale )
{
    _duration = duration;
    _scale = scale;
    return true;
}
void scale_to::setup( )
{
    _start_scale = _target.lock( )->get_scale( );
}
void scale_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );

    auto const to = _scale;
    auto const from = _start_scale;
    auto const temp = vec2( ease_liner( t, from.x, to.x ), ease_liner( t, from.y, to.y ) );
    _target.lock( )->set_scale( temp );
}
}