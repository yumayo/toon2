#include "move_to_target.h"
#include "node.h"
using namespace cinder;
namespace action
{
CREATE_CPP( move_to_target, float duration, std::shared_ptr<node> const& target )
{
    CREATE( move_to_target, duration, target );
}
bool move_to_target::init( float duration, std::shared_ptr<node> const& target )
{
    _duration = duration;
    _new_target = target;
    return true;
}
void move_to_target::setup( )
{
    _start_position = _target.lock( )->get_position( );
}
void move_to_target::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );

    auto const to = _new_target.lock( )->get_position( );
    auto const from = _start_position;
    auto const temp = vec2( ease_liner( t, from.x, to.x ), ease_liner( t, from.y, to.y ) );
    _target.lock( )->set_position( temp );
}
}