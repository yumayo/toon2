#pragma once
#include "cinder/app/App.h"
class ty : public cinder::app::App
{
public:
    void setup( ) override;
    void cleanup( ) override;
    void mouseDown( cinder::app::MouseEvent event ) override;
    void mouseUp( cinder::app::MouseEvent event ) override;
    void mouseWheel( cinder::app::MouseEvent event ) override;
    void mouseMove( cinder::app::MouseEvent event ) override;
    void mouseDrag( cinder::app::MouseEvent event ) override;
    void touchesBegan( cinder::app::TouchEvent event ) override;
    void touchesMoved( cinder::app::TouchEvent event ) override;
    void touchesEnded( cinder::app::TouchEvent event ) override;
    void update( ) override;
    void draw( ) override;
private:
};