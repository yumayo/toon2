#pragma once
#include "bullet.h"
#include <treelike/renderer/circle.h>
#include "spike.h"
namespace user
{
class bullet_straight : public bullet
{
public:
    CREATE_H( bullet_straight, int bullet_id, cinder::vec2 start_position, cinder::vec2 end_position, cinder::Color color, std::string const& skin_relative_path );
    bool init( int bullet_id, cinder::vec2 start_position, cinder::vec2 end_position, cinder::Color color, std::string const& skin_relative_path );
protected:
    softptr<spike> _base;
    softptr<treelike::renderer::circle> _mask;
};
}