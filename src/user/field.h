#pragma once
#include <treelike/node.h>
#include <jsoncpp/json.h>
#include "synchronization_objects.h"
namespace user
{
class field : public treelike::node
{
    softptr<treelike::node> _ground;
    softptr<treelike::node> _feed_manager;
    softptr<treelike::node> _bullet_manager;
    softptr<treelike::node> _cell_manager;
    softptr<treelike::node> _player;
public:
    CREATE_H( field,
              Json::Value& root,
              std::vector<feed_data> feed_buffet,
              Json::Value& bullet_buffer,
              std::vector<std::vector<ground_data>>& ground_buffer );
    bool init( Json::Value& root,
               std::vector<feed_data> feed_buffet,
               Json::Value& bullet_buffer,
               std::vector<std::vector<ground_data>>& ground_buffer );
    inline softptr<treelike::node> get_ground( ) { return _ground; }
    inline softptr<treelike::node> get_feed_manager( ) { return _feed_manager; }
    inline softptr<treelike::node> get_bullet_manager( ) { return _bullet_manager; }
    inline softptr<treelike::node> get_cell_manager( ) { return _cell_manager; }
    inline softptr<treelike::node> get_player( ) { return _player; }
};
}