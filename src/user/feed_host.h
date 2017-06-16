#pragma once
#include "feed.h"
namespace user
{
class feed_host : public feed
{
public:
    CREATE_H( feed_host );
    bool init( );
public:
    void on_captured( std::weak_ptr<node> other ) final override;
};
}