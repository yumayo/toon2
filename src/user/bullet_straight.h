#pragma once
#include "bullet.h"
namespace user
{
class bullet_straight : public bullet
{
public:
    CREATE_H( bullet_straight, int bullet_id, float time, float radius, cinder::vec2 start_position, cinder::vec2 end_position, cinder::Color color, std::string const& skin_relative_path );
    bool init( int bullet_id, float time, float radius, cinder::vec2 start_position, cinder::vec2 end_position, cinder::Color color, std::string const& skin_relative_path );
};
}