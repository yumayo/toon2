#pragma once
#include <treelike/node.h>
#include <treelike/renderer/circle.h>
#include "spike.h"
namespace user
{
class bullet : public treelike::node
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
    softptr<spike> _base;
    softptr<treelike::renderer::circle> _mask;
    cinder::vec2 _direction = cinder::vec2( 0 );
};
}