#pragma once
#include "../node.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
namespace renderer
{
class label : public node
{
public:
    CREATE_H( label, std::string const& text, std::string const& relative_path, float size );
    bool init( );
    bool init( std::string const& text, std::string const& relative_path, float size );
    void render( ) override;
private:
    cinder::TextLayout _layout;
    cinder::gl::TextureRef _texture;
public:
    LUA_SETUP_H( label );
};
}