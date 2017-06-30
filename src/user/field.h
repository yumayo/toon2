#pragma once
#include "node.h"
#include <jsoncpp/json.h>
#include "synchronization_objects.h"
namespace user
{
class field : public node
{
    std::weak_ptr<node> _player;
    std::weak_ptr<node> _bullet_manager;
public:
    CREATE_H( field, 
              Json::Value& root,
              std::vector<feed_data> feed_buffet,
              std::vector<bullet_data>& bullet_buffer,
              std::vector<std::vector<ground_data>>& ground_buffer );
    bool init( Json::Value& root,
               std::vector<feed_data> feed_buffet,
               std::vector<bullet_data>& bullet_buffer,
               std::vector<std::vector<ground_data>>& ground_buffer );
    void render( ) override;
    std::weak_ptr<node> get_bullet_manager( );
};
}