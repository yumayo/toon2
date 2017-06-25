#pragma once
#include "node.h"
#include "network.hpp"
namespace user
{
class bullet_manager : public node
{
public:
    CREATE_H( bullet_manager, std::weak_ptr<node> cell_manager );
    bool init( std::weak_ptr<node> cell_manager );
    void update( float delta ) final override;
protected:
    std::weak_ptr<node> _cell_manager;
    std::weak_ptr<network::tcp_client> _tcp_connection;
};
}