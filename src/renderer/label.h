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
    bool init( std::string const& text, std::string const& relative_path, float size );
    void render( ) override;
public:
    void set_text( std::string const& text );
private:
    cinder::TextLayout _layout;
    cinder::gl::TextureRef _texture;
    std::string _relative_path;
    float _size;
public:
    LUA_SETUP_H( label );
};
}