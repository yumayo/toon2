#pragma once
#include "cocoslike.h"
#include "jsoncpp/json.h"
#include "../network/udp_client.h"
namespace user
{
class search_room : public node
{
public:
    CREATE_H( search_room );
    bool init( );
public:
    std::function<void( Json::Value& root )> on_founded;
    std::function<void( )> on_not_found;
};
}