#pragma once
#include <treelike/node.h>
#include "player.h"
#include "ground.h"
#include "bullet_manager.h"
#include "analog_axis.h"
#include "debug_command.h"
namespace user
{
class controller : public treelike::node
{
public:
    CREATE_H( controller, softptr<player> player, softptr<ground> ground, softptr<bullet_manager> bullet_manager );
    bool init( softptr<player> player, softptr<ground> ground, softptr<bullet_manager> bullet_manager );
    void update( float delta ) override;
private:
    softptr<player> _player;
    softptr<ground> _ground;
    softptr<bullet_manager> _bullet_manager;
    softptr<analog_axis> _analog_axis;
    softptr<debug_command> _debug_command;
};
}