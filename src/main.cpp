#include "app_delegate.h"
#include "cinder/app/RendererGl.h"

#ifdef _DEBUG
CINDER_APP( app_delegate, cinder::app::RendererGl, [ & ] ( cinder::app::App::Settings *settings )
{
    //settings->setConsoleWindowEnabled( );
    //settings->setWindowPos( -1600, 180 );
    settings->setWindowSize( 1280, 720 );
    settings->setTitle( u8"toon2" );
    settings->setResizable( false );
} )
#else
CINDER_APP( app_delegate, cinder::app::RendererGl, [ & ] ( cinder::app::App::Settings *settings )
{
    //settings->setConsoleWindowEnabled( );
    //settings->setWindowPos( -1600, 180 );
    settings->setWindowSize( 1280, 720 );
    settings->setTitle( u8"toon2" );
    settings->setResizable( false );
} )
#endif