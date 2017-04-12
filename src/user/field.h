#pragma once
#include "../node.h"
#include <jsoncpp/json.h>
namespace user
{
class field : public node
{
public:
    CREATE_H( field, Json::Value& root );
    bool init( Json::Value& root );
};
}