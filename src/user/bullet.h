#pragma once
#include "node.h"
#include "renderer/circle.h"
#include "spike.h"
namespace user
{
class bullet : public node
{
public:
    bool init( int bullet_id, float time_offset, cinder::vec2 position, cinder::vec2 direction, cinder::Color color, std::string const& skin_relative_path );
    void update( float delta ) override;
    float get_radius( );
    bool is_hit( );
    void hit( );
protected:
    bool _is_hit = false;
    float _radius = 20.0F;
    std::weak_ptr<spike> _base;
    std::weak_ptr<renderer::circle> _mask;
    cinder::vec2 _direction = cinder::vec2( 0 );
};
}