#include "scale_by.h"
#include "node.h"
using namespace cinder;
namespace action
{
CREATE_CPP( scale_by, float duration, cinder::vec2 scale )
{
    CREATE( scale_by, duration, scale );
}
bool scale_by::init( float duration, cinder::vec2 scale )
{
    _duration = duration;
    _init_scale = scale;
    return true;
}
void scale_by::setup( )
{
    _start_scale = _target.lock( )->get_scale( );
    _scale = _init_scale + _start_scale;
}
}