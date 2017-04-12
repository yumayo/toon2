#pragma once
#include "cocoslike.h"
#include "field.h"
#include "player.h"
#include "controller.h"
#include "ground.h"
namespace user
{
class game : public scene
{
public:
    CREATE_H( game, Json::Value root );
    bool init( Json::Value& root );
    void update( float delta ) override;
private:
    std::weak_ptr<field> _field;
    std::weak_ptr<player> _own;
    std::weak_ptr<ground> _ground;
    std::weak_ptr<controller> _controller;
};
}