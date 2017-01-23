#include "app_delegate.h"
#include "cinder/app/RendererGl.h"

CINDER_APP( app_delegate, cinder::app::RendererGl, [ & ] ( cinder::app::App::Settings *settings )
{
    settings->setWindowSize( 640, 360 );
} )