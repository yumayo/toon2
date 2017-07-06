#pragma once
#include <treelike/node.h>
#include "player.h"
#include "enemy.h"
#include "jsoncpp/json.h"
namespace user
{
class cell_manager : public treelike::node
{
public:
    CREATE_H( cell_manager, Json::Value& root );
    bool init( Json::Value& root );
    virtual void update( float delta )override;
public:
    std::list<softptr<enemy>>& get_enemys( );
    softptr<player>& get_player( );
    void set_ground( softptr<treelike::node> ground );
    void set_bullet_manager( softptr<treelike::node> bullet_manager );
    void remove_all_crown( );
    void set_all_crown( std::vector<int> const& ids );
protected:
    std::list<softptr<enemy>> _enemys;
    softptr<player> _player;
    softptr<treelike::network::udp_connection> _udp_connection;
    softptr<treelike::network::tcp_client> _tcp_connection;
    softptr<treelike::node> _ground;
    softptr<treelike::node> _bullet_manager;
    void create_player( Json::Value const& data );
    void create_enemy( Json::Value const& data );
};
}