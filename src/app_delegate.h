#pragma once
#include "node.h"
class app_delegate : public node, public virtual cinder::app::App
{
public:
    app_delegate( );
    ~app_delegate( );
public:
    void setup( ) override;
    void cleanup( ) override;
    void update( ) override;
    void draw( ) override;
    void mouseDown( cinder::app::MouseEvent event ) override;
    void mouseUp( cinder::app::MouseEvent event ) override;
    void mouseMove( cinder::app::MouseEvent event ) override;
    void mouseDrag( cinder::app::MouseEvent event ) override;
    void touchesBegan( cinder::app::TouchEvent event ) override;
    void touchesMoved( cinder::app::TouchEvent event ) override;
    void touchesEnded( cinder::app::TouchEvent event ) override;
protected:
    double _prev_second = 0.0;
};