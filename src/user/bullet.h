#pragma once
#include "node.h"
#include "renderer/circle.h"
#include "spike.h"
namespace user
{
class bullet : public node
{
public:
    CREATE_H( bullet, int tag, cinder::vec2 position, cinder::vec2 direction, std::weak_ptr<node> cell );
    bool init( int tag, cinder::vec2 position, cinder::vec2 direction, std::weak_ptr<node> cell );
    void update( float delta ) override;
    float get_radius( );
protected:
    float _radius = 20.0F;
    std::weak_ptr<spike> _base;
    std::weak_ptr<renderer::circle> _mask;
    cinder::vec2 _direction = cinder::vec2( 0 );
};
}