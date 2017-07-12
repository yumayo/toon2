#pragma once
#include <jsoncpp/json.h>
#include <treelike/node.h>
namespace user
{
namespace parser
{
void bullet_struct( Json::Value * root, int user_id, float time, float radius, cinder::vec2 start_position, cinder::vec2 end_position );
void bullet_tip( Json::Value * root, int user_id, cinder::vec2 start_position, cinder::vec2 direction );
bool bullet_straight( Json::Value* root, softptr<treelike::node> n_player, softptr<treelike::node> n_analog_axis );
bool bullet_fireworks( Json::Value* root, softptr<treelike::node> n_player, softptr<treelike::node> n_analog_axis );
void bullet_fireworks_tip( Json::Value* root, int user_id, cinder::vec2 start_position, cinder::vec2 end_position );
}
}