#pragma once
#include "../node.h"
#include "cinder/gl/Texture.h"
namespace user
{
    class sprite_nearest : public node
    {
    public:
        CREATE_H( sprite_nearest, std::string const& relative_path );
        bool init( std::string const& relative_path );
        void render( ) override;
    private:
        cinder::gl::TextureRef _texture;
    };
}