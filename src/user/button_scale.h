#pragma once
#include "node.h"
#include "cinder/gl/Texture.h"
namespace user
{
    class button_scale : public node
    {
    public:
        CREATE_H( button_scale );
        bool init( );
        void update( float delta ) override;
        virtual bool mouse_began( cinder::app::MouseEvent event ) override;
        virtual void mouse_moved( cinder::app::MouseEvent event ) override;
        virtual void mouse_ended( cinder::app::MouseEvent event ) override;
        virtual bool touch_began( cinder::app::TouchEvent::Touch event )override;
        virtual void touch_moved( cinder::app::TouchEvent::Touch event )override;
        virtual void touch_ended( cinder::app::TouchEvent::Touch event )override;
    public:
        std::function<void( )> on_began;
        std::function<void( )> on_ended;
    protected:
        bool _touch = false;
        bool _prev_touch = false;
    };
}
