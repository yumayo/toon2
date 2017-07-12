#pragma once
#include <treelike/renderer/circle.h>
namespace user
{
class soft_circle : public treelike::renderer::circle
{
public:
    CREATE_H( soft_circle, float radius );
    bool init( float radius );
    virtual void render( ) override;
    virtual void update( float delta ) override;
protected:
    std::vector<float> _edges;
};
}