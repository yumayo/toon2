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
        void render( ) override;
        bool mouse_began( cinder::app::MouseEvent event ) override;
        void mouse_moved( cinder::app::MouseEvent event ) override;
        void mouse_ended( cinder::app::MouseEvent event ) override;
    private:
        bool _touch = false;
    };
}