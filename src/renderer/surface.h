#pragma once
#include "../node.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
namespace renderer
{
    class surface : public node
    {
    public:
        CREATE_H( surface, cinder::vec2 size );
        bool init( cinder::vec2 size );
        void render( ) override;
    public:
        void set_pixel( cinder::vec2 pixel, cinder::ColorA color );
        cinder::ColorA get_pixel( cinder::vec2 pixel );
    private:
        cinder::Surface32f _surface;
        cinder::gl::TextureRef _texture;
    public:
        LUA_SETUP_H( surface );
    };
}