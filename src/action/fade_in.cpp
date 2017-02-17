#include "fade_in.h"
#include "../node.h"
using namespace cinder;
namespace action
{
CREATE_CPP( fade_in, float duration )
{
    CREATE( fade_in, duration );
}
bool fade_in::init( float duration )
{
    finite_time_action::init( duration );
    return true;
}
void fade_in::setup( )
{

}
void fade_in::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );

    auto const to = 1.0F;
    auto const from = 0.0F;
    auto const temp = ease_liner( t, from, to );
    _target.lock( )->set_opacity( temp );
}

#define l_class fade_in
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( fade_in
           , l_base( finite_time_action )
           , l_set( create )
    );
}
#include "lua_undef.h"
}