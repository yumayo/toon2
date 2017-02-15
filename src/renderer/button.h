#pragma once
#include "../node.h"
namespace renderer
{
    class button : public node
    {
    public:
        CREATE_H( button, cinder::vec2 const& size );
        CREATE_H( button, float x, float y );
        bool init( cinder::vec2 const& size );
        virtual void render( ) override;
        virtual bool mouse_began( cinder::app::MouseEvent event ) override;
        virtual void mouse_moved( cinder::app::MouseEvent event ) override;
        virtual void mouse_ended( cinder::app::MouseEvent event ) override;
        virtual bool touch_began( cinder::app::TouchEvent::Touch event )override;
        virtual void touch_moved( cinder::app::TouchEvent::Touch event )override;
        virtual void touch_ended( cinder::app::TouchEvent::Touch event )override;
    protected:
        bool _touch = false;
    private:
        bool hit_point( cinder::vec2 point );
    public:
        LUA_SETUP_H( button );
    };
}
