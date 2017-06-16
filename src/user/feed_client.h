#pragma once
#include "feed.h"
#include "network.hpp"
namespace user
{
class feed_client : public feed
{
public:
    CREATE_H( feed_client );
    bool init( );
public:
    void on_captured( std::weak_ptr<node> other ) final override;
private:
    std::weak_ptr<network::udp_connection> _udp_connection;
};
}