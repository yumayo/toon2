#include "toon2.h"
#include "cinder/gl/gl.h"
using namespace cinder;
void toon2::setup( )
{
}
void toon2::cleanup( )
{
}
void toon2::mouseDown( cinder::app::MouseEvent event )
{
}
void toon2::mouseUp( cinder::app::MouseEvent event )
{
}
void toon2::mouseWheel( cinder::app::MouseEvent event )
{
}
void toon2::mouseMove( cinder::app::MouseEvent event )
{
}
void toon2::mouseDrag( cinder::app::MouseEvent event )
{
}
void toon2::touchesBegan( cinder::app::TouchEvent event )
{
}
void toon2::touchesMoved( cinder::app::TouchEvent event )
{
}
void toon2::touchesEnded( cinder::app::TouchEvent event )
{
}
void toon2::update( )
{
    console( ) << getElapsedSeconds( ) - prev_second << std::endl;
    prev_second = getElapsedSeconds( );
}
void toon2::draw( )
{
}