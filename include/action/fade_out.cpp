#include "fade_out.h"
#include "node.h"
using namespace cinder;
namespace action
{
CREATE_CPP( fade_out, float duration )
{
    CREATE( fade_out, duration );
}
bool fade_out::init( float duration )
{
    finite_time_action::init( duration );
    return true;
}
void fade_out::setup( )
{
    _opacity = _target.lock( )->get_opacity( );
}
void fade_out::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );

    auto const to = 0.0F;
    auto const from = _opacity;
    auto const temp = ease_liner( t, from, to );
    _target.lock( )->set_opacity( temp );
}
}