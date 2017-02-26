#pragma once
#include "captured_object.h"
#include "../renderer/circle.h"
#include "toon_packet.h"
#include "cinder/gl/Texture.h"
namespace user
{
class player : public captured_object
{
public:
    CREATE_H( player, cinder::ColorA color );
    bool init( cinder::ColorA color );
    CREATE_H( player, cinder::ColorA color, std::string const& relative_path_skin );
    bool init( cinder::ColorA color, std::string const& relative_path_skin );
    virtual ~player( );
public:
    float get_radius( );
    void set_radius( float value );
    void on_captured( std::weak_ptr<node> other ) override;
    void capture( float score );
    void move( cinder::vec2 axis );
protected:
    std::weak_ptr<renderer::circle> _base;
    std::weak_ptr<renderer::circle> _mask;
    float _radius = 0.0F;
    float _target_radius = 0.0F;
    float _setup_radius = 20.0F;
};
}