#pragma once
#include "node.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
namespace user
{
class dot_label : public node
{
public:
    CREATE_H( dot_label, std::string const& text, std::string const& relative_path, float size );
    bool init( std::string const& text, std::string const& relative_path, float size );
    void render( ) override;
public:
    void set_text( std::string const& text );
private:
    std::shared_ptr<cinder::TextLayout> _layout;
    cinder::Font _font;
    cinder::gl::TextureRef _texture;
    std::string _relative_path;
    float _size;
};
}