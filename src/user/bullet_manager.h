#pragma once
#include "node.h"
#include "network.hpp"
#include "jsoncpp/json.h"
namespace user
{
class bullet_manager : public node
{
public:
    CREATE_H( bullet_manager, std::weak_ptr<node> cell_manager );
    bool init( std::weak_ptr<node> cell_manager );
    void update( float delta ) final override;
    void close_player( cinder::ColorA const& color );
    void create_bullet( Json::Value const& data );
protected:
    std::weak_ptr<node> _cell_manager;
    std::weak_ptr<network::tcp_client> _tcp_connection;
    int _number_of_created_bullet = 0;
    Json::Value _created_bullet_data;
    float _time_offset = 0.0F;
};
}