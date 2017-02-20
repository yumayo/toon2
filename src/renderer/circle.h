#pragma once
#include "../node.h"

namespace renderer
{
    class circle : public node
    {
    public:
        CREATE_H( circle, float radius, float segments = -1.0F );
        bool init( float radius, float segments = -1.0F );
        void render( ) override;
    public:
        void set_radius( float value );
        float get_radius( );
    protected:
        float _radius = 0.0F;
        float _segments = -1.0F;
    public:
        LUA_SETUP_H( circle );
    };
}