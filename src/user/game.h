#pragma once
#include "cocoslike.h"
#include "field.h"
#include "player.h"
#include "controller.h"
namespace user
{
class game : public node
{
public:
    CREATE_H( game );
    bool init( );
    void update( float delta ) override;
private:
    sol::state* lua;
    std::weak_ptr<field> _field;
    std::weak_ptr<player> _player;
    std::weak_ptr<controller> _controller;
};
}