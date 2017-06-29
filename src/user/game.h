#pragma once
#include "cocoslike.hpp"
#include "field.h"
#include "player.h"
#include "controller.h"
#include "ground.h"
namespace user
{
class game : public scene
{
public:
    CREATE_H( game, Json::Value& root, std::map<int, cinder::ivec2>& feeds_buffer, std::vector<std::vector<unsigned char>>& ground_buffer );
    ~game( );
    bool init( Json::Value& root, std::map<int, cinder::ivec2>& feeds_buffer, std::vector<std::vector<unsigned char>>& ground_buffer );
    void update( float delta ) override;
    void render( ) override;
private:
    std::weak_ptr<field> _field;
    std::weak_ptr<player> _player;
    std::weak_ptr<ground> _ground;
    std::weak_ptr<controller> _controller;
    std::weak_ptr<node> _back_button;
};
}