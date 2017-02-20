#pragma once
#include "player.h"
namespace user
{
class own : public player
{
public:
    CREATE_H( own );
    bool init( );
private:
    std::weak_ptr<node> _udp_sender;
};
}