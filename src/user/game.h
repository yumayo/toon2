#pragma once
#include <treelike.hpp>
#include "field.h"
#include "player.h"
#include "controller.h"
#include "ground.h"
#include "synchronization_objects.h"
namespace user
{
class game : public treelike::scene
{
public:
    CREATE_H( game, 
              Json::Value& root,
              std::vector<feed_data> feed_buffet,
              Json::Value& bullet_buffer,
              std::vector<std::vector<ground_data>>& ground_buffer );
    ~game( );
    bool init( Json::Value& root,
               std::vector<feed_data> feed_buffet,
               Json::Value& bullet_buffer,
               std::vector<std::vector<ground_data>>& ground_buffer );
    void update( float delta ) override;
private:
    softptr<field> _field;
    softptr<player> _player;
    softptr<ground> _ground;
    softptr<controller> _controller;
    softptr<node> _back_button;
};
}