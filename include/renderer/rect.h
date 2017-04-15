#pragma once
#include "node.h"

namespace renderer
{
    class rect : public node
    {
    public:
        CREATE_H( rect, cinder::vec2 const& size );
        bool init( cinder::vec2 const& size );
        void render( ) override;
    };
}