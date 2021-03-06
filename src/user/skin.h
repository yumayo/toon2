#pragma once
#include <treelike.hpp>
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Batch.h"
namespace user
{
class skin : public treelike::renderer::circle
{
public:
    CREATE_H( skin, float radius, float segments, std::string const& relative_path );
    bool init( float radius, float segments, std::string const& relative_path );
    void render( ) override;
private:
    cinder::gl::TextureRef _texture;
    cinder::gl::GlslProgRef _glsl;
};
}