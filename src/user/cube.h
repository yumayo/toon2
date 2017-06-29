#pragma once
#include "node.h"
#include "cinder/gl/Shader.h"
namespace user
{
class cube : public node
{
public:
    CREATE_H( cube, cinder::vec3 const& size );
    bool init( cinder::vec3 const& size );
    void render( ) override;
private:
    cinder::vec3 _size;
    cinder::gl::GlslProgRef _shader;
};
}