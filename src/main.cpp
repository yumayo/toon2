#include <treelike/app_delegate.h>
#include "user/title.h"
namespace treelike
{
void setting( cinder::app::App::Settings *settings )
{
    settings->setConsoleWindowEnabled( );
    //settings->setWindowPos( -1600, 180 );
    settings->setWindowSize( 1280, 720 );
    settings->setTitle( u8"toon2" );
    settings->setResizable( false );
}
hardptr<scene> main( )
{
    return user::title::create( );
}
}