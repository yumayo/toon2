#pragma once
#include "bullet.h"
namespace user
{
class bullet_fireworks : public bullet
{
public:
    CREATE_H( bullet_fireworks, 
              int bullet_id,
              float time,
              float radius,
              cinder::vec2 start_position,
              cinder::vec2 end_position,
              cinder::Color color,
              std::string const& skin_relative_path,
              std::function<void()> fire_action );
    bool init( int bullet_id,
               float time,
               float radius,
               cinder::vec2 start_position,
               cinder::vec2 end_position,
               cinder::Color color,
               std::string const& skin_relative_path,
               std::function<void( )> fire_action );
};
}