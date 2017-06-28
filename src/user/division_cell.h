#pragma once
#include "renderer/circle.h"
#include "captured_object.h"
namespace user
{
class division_cell : public captured_object
{
public:
    CREATE_H( division_cell, int tag, cinder::vec2 position, cinder::vec2 direction, float radius, cinder::Color const& color, std::string const& skin_relative_path );
    bool init( int tag, cinder::vec2 position, cinder::vec2 direction, float radius, cinder::Color const& color, std::string const& skin_relative_path );
public:
    float get_radius( );
    void on_captured( std::weak_ptr<node> other ) final override;
protected:
    std::weak_ptr<renderer::circle> _circle;
    float _score = 0.0F;
};
}