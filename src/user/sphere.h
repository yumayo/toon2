#pragma once
#include <treelike/node.h>
#include "cinder/gl/Shader.h"
namespace user
{
class sphere : public treelike::node
{
public:
    CREATE_H( sphere, float radius, float subdivisions = -1.0F );
    virtual ~sphere( );
    bool init( float radius, float subdivisions = -1.0F );
    virtual void render( ) override;
public:
    void set_radius( float value );
    float get_radius( );
    void set_subdivisions( float value );
    float get_subdivisions( );
protected:
    float _radius = 0.0F;
    float _subdivisions = -1.0F;
    cinder::gl::GlslProgRef _shader;
};
}