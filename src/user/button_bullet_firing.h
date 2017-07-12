#pragma once
#include <treelike/node.h>
#include "button.h"
#include "player.h"
#include "bullet_manager.h"
namespace user
{
// キーボードのスペースでも機能するようにしたボタンを作成します。
class button_bullet_firing_function : public circle_button_opacity_action
{
public:
    CREATE_H( button_bullet_firing_function );
    bool init( );
private:
    void key_down( cinder::app::KeyEvent e ) override;
    void on_key_event( int code, std::function<void( cinder::app::KeyEvent )> func );
private:
    std::map<int, std::function<void( cinder::app::KeyEvent )>> _key_events;
};
class button_bullet_firing : public treelike::node
{
public:
    CREATE_H( button_bullet_firing, softptr<player> player, softptr<bullet_manager> bullet_manager );
    bool init( softptr<player> player, softptr<bullet_manager> bullet_manager );
    void update( float delta );
private:
    softptr<button_bullet_firing_function> _circle_button;
    softptr<player> _player;
    softptr<bullet_manager> _bullet_manager;
    softptr<treelike::node> _base;
    softptr<treelike::node> _mask;
};
}