#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class gameApp : public App {
public:
    void setup( ) override;
    void mouseDown( MouseEvent event ) override;
    void update( ) override;
    void draw( ) override;
};



void gameApp::setup( )
{
}

void gameApp::mouseDown( MouseEvent event )
{
}

void gameApp::update( )
{
}

void gameApp::draw( )
{
    gl::clear( Color( 0, 0, 0 ) );

    gl::drawSolidRect( Rectf( 0, 0, 100, 100 ) );

}

CINDER_APP( gameApp, RendererGl, [ & ] ( App::Settings *settings ) 
{
    settings->setWindowSize( 1280, 720 );
} )
