#pragma once
#include <treelike/node.h>
#include <cinder/gl/GlslProg.h>
namespace user
{
class render3d : public treelike::node
{
public:
    CREATE_H( render3d );
    bool init( );
    void _render( cinder::Camera const& camera, cinder::mat4 m ) override;
private:
    cinder::gl::GlslProgRef _glsl;
};
}
