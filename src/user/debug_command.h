#pragma once
#include <treelike/node.h>
#include "player.h"
#include "bullet_manager.h"
namespace user
{
class debug_command : public treelike::node
{
public:
    CREATE_H( debug_command, softptr<player> player, softptr<bullet_manager> bullet_manager );
    bool init( softptr<player> player, softptr<bullet_manager> bullet_manager );
    void key_down( cinder::app::KeyEvent e ) override;
private:
    void on_key_event( int code, std::function<void( )> func );
private:
    softptr<player> _player;
    softptr<bullet_manager> _bullet_manager;
    std::map<int, std::function<void( )>> _key_events;
};
}