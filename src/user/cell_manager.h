#pragma once
#include "node.h"
#include "player.h"
#include "enemy.h"
namespace user
{
class cell_manager : public node
{
public:
    CREATE_H( cell_manager, Json::Value& root );
    bool init( Json::Value& root );
    virtual void update( float delta )override;
public:
    std::list<std::weak_ptr<enemy>>& get_enemys( );
    std::weak_ptr<player>& get_player( );
    void set_ground( std::weak_ptr<node> ground );
    void remove_all_crown( );
    void set_all_crown( std::vector<int> const& ids );
protected:
    std::list<std::weak_ptr<enemy>> _enemys;
    std::weak_ptr<player> _player;
    std::weak_ptr<network::udp_connection> _udp_connection;
    std::weak_ptr<network::tcp_client> _tcp_connection;
    std::weak_ptr<node> _ground;
    void create_player( Json::Value const& data );
    void create_enemy( Json::Value const& data );
};
}