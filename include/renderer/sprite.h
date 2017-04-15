#pragma once
#include "node.h"
#include "cinder/gl/Texture.h"
namespace renderer
{
    class sprite : public node
    {
    public:
        CREATE_H( sprite, std::string const& relative_path );
        bool init( std::string const& relative_path );
        void render( ) override;
    private:
        cinder::gl::TextureRef _texture;
    };
}