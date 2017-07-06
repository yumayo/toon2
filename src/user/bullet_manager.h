#pragma once
#include <treelike/node.h>
#include <treelike/network.hpp>
#include "jsoncpp/json.h"
namespace user
{
class bullet_manager : public treelike::node
{
public:
    CREATE_H( bullet_manager, softptr<treelike::node> cell_manager, Json::Value bullet_buffer );
    bool init( softptr<treelike::node> cell_manager, Json::Value bullet_buffer );
    void update( float delta ) final override;
    void close_player( cinder::ColorA const& color );
    void create_bullet( Json::Value const& data );
protected:
    softptr<treelike::node> _cell_manager;
    softptr<treelike::network::tcp_client> _tcp_connection;
    int _number_of_created_bullet = 0;
    Json::Value _created_bullet_data;
    float _time_offset = 0.0F;
};
}