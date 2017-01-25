#pragma once
#include "cocoslike.h"
#include "rect.h"

namespace renderer
{
    class button : public node
    {
    public:
        CREATE_H( button, cinder::vec2 const& size );
        bool init( cinder::vec2 const& size );
        virtual void render( ) override;
        virtual bool mouse_began( cinder::app::MouseEvent event ) override;
        virtual void mouse_moved( cinder::app::MouseEvent event ) override;
        virtual void mouse_ended( cinder::app::MouseEvent event ) override;
        virtual bool touches_began( cinder::app::TouchEvent event )override;
        virtual void touches_moved( cinder::app::TouchEvent event )override;
        virtual void touches_ended( cinder::app::TouchEvent event )override;
    protected:
        bool _swallow = false;
        bool _touch = false;
    };
}
