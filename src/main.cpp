#include "toon2.h"

CINDER_APP( toon2, cinder::app::RendererGl, [ & ] ( cinder::app::App::Settings *settings )
{
    settings->setWindowSize( 640, 360 );
} )