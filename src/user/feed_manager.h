#pragma once
#include <treelike/node.h>
#include <treelike/network.hpp>
#include "jsoncpp/json.h"
#include "synchronization_objects.h"
namespace user
{
class feed_manager : public treelike::node
{
public:
    CREATE_H( feed_manager, softptr<treelike::node> cell_manager, std::vector<feed_data>& feed_buffer );
    bool init( softptr<treelike::node> cell_manager, std::vector<feed_data>& feed_buffer );
    void update( float delta ) final override;
    void on_feed_captured( int tag );
protected:
    int _captured_feed_number = 0;
    Json::Value _captured_feed_data;
    softptr<treelike::node> _cell_manager;
    softptr<treelike::network::tcp_client> _tcp_connection;
};
}