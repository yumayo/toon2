#pragma once
#include "node.h"
#include <jsoncpp/json.h>
namespace user
{
class field : public node
{
public:
    CREATE_H( field, Json::Value& root, std::map<int, cinder::ivec2>& feeds_buffer, std::vector<std::vector<unsigned char>>& ground_buffer );
    bool init( Json::Value& root, std::map<int, cinder::ivec2>& feeds_buffer, std::vector<std::vector<unsigned char>>& ground_buffer );
};
}