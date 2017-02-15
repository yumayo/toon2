#pragma once
#include "node.h"
class app_delegate : public cinder::app::App
{
private:
    void setup( ) override;
    void update( ) override;
    void draw( ) override;
    void mouseDown( cinder::app::MouseEvent event ) override;
    void mouseDrag( cinder::app::MouseEvent event ) override;
    void mouseUp( cinder::app::MouseEvent event ) override;
    void touchesBegan( cinder::app::TouchEvent event ) override;
    void touchesMoved( cinder::app::TouchEvent event ) override;
    void touchesEnded( cinder::app::TouchEvent event ) override;
    void keyDown( cinder::app::KeyEvent event ) override;
private:
    void lua_run( );
private:
    static int const _INVALID_ID;
    std::shared_ptr<sol::state> _lua;
    std::shared_ptr<node> _root;
    int _touch_id = _INVALID_ID;
    double _prev_second = 0.0;
};