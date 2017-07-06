#pragma once
#include <treelike/renderer/circle.h>
namespace user
{
class spike : public treelike::renderer::circle
{
public:
    CREATE_H( spike, float radius, float segments = -1.0F, float sharpness = 3.0F);
    bool init( float radius, float segments = -1.0F, float sharpness = 3.0F );
    virtual void render( ) override;
public:
    void set_sharpness( float value );
    float get_sharpness( );
protected:
    float _sharpness = 3.0F;
};
}