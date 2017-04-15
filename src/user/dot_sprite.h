#pragma once
#include "node.h"
#include "cinder/gl/Texture.h"
namespace user
{
    class dot_sprite : public node
    {
    public:
        CREATE_H( dot_sprite, std::string const& relative_path );
        bool init( std::string const& relative_path );
        void render( ) override;
    private:
        cinder::gl::TextureRef _texture;
    };
}