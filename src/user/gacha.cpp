#include "gacha.h"
#include "create_dot_obeject.h"
namespace user
{
CREATE_CPP( gacha )
{
    CREATE( gacha );
}
bool gacha::init( )
{
    auto dot = create_dot( "gacha_maschine.png", 600 );
    dot->set_position( cinder::vec2( cinder::app::getWindowSize( ) ) * 0.5F );
    add_child( dot );

    return true;
}
}

