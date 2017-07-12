#pragma once
#include <jsoncpp/json.h>
#include <treelike/node.h>
namespace user
{
namespace parser
{
bool bullet_straight( Json::Value* root, softptr<treelike::node> n_player, softptr<treelike::node> n_analog_axis );
}
}