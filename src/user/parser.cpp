#include "parser.h"
#include "player.h"
#include "analog_axis.h"
#include <treelike/user_default.h>
#include <cinder/Rand.h>
using namespace cinder;
using namespace treelike;
namespace user
{
namespace parser
{
void bullet_struct( Json::Value * root, int user_id, float time, float radius, cinder::vec2 start_position, cinder::vec2 end_position )
{
    Json::Value& data = *root;
    data["user_id"] = user_id;
    data["time"] = time;
    data["radius"] = radius;
    data["start_position"][0] = start_position.x;
    data["start_position"][1] = start_position.y;
    data["end_position"][0] = end_position.x;
    data["end_position"][1] = end_position.y;
}
void bullet_tip( Json::Value * root, int user_id, cinder::vec2 start_position, cinder::vec2 direction )
{
    auto end_position = start_position + direction * 800.0F;
    bullet_struct( root, user_id, 2.0F, 20.0F, start_position, end_position );
}
bool bullet_straight( Json::Value * root, softptr<treelike::node> n_player, softptr<treelike::node> n_analog_axis )
{
    float energy = 5.0F;

    auto player = n_player.dynamicptr<user::player>( );
    auto analog_axis = n_analog_axis.dynamicptr<user::analog_axis>( );

    // ¬‚³‚¢ê‡‚Í‘Å‚Ä‚È‚¢B
    if ( player->get_target_radius( ) < user_default::get_instans( )->get_root( )["system"]["min_radius"].asFloat( ) + energy ) return false;

    auto start_position = player->get_position( ) + player->get_radius( ) * analog_axis->get_last_normalized_axis( );
    auto end_position = start_position + 800.0F * analog_axis->get_last_normalized_axis( );

    player->scale_action( -energy );

    bullet_struct( root, player->get_tag( ), 2.0F, 20.0F, start_position, end_position );

    return true;
}
bool bullet_fireworks( Json::Value * root, softptr<treelike::node> n_player, softptr<treelike::node> n_analog_axis )
{
    float energy = 10.0F;

    auto player = n_player.dynamicptr<user::player>( );
    auto analog_axis = n_analog_axis.dynamicptr<user::analog_axis>( );

    // ¬‚³‚¢ê‡‚Í‘Å‚Ä‚È‚¢B
    if ( player->get_target_radius( ) < user_default::get_instans( )->get_root( )["system"]["min_radius"].asFloat( ) + energy ) return false;

    auto start_position = player->get_position( ) + player->get_radius( ) * analog_axis->get_last_normalized_axis( );
    auto end_position = start_position + 300.0F * analog_axis->get_last_normalized_axis( );

    player->scale_action( -energy );

    bullet_struct( root, player->get_tag( ), 1.0F, 40.0F, start_position, end_position );
    return true;
}
void bullet_fireworks_tip( Json::Value * root, int user_id, cinder::vec2 start_position, cinder::vec2 direction )
{
    auto end_position = start_position + direction * 400.0F;
    bullet_struct( root, user_id, 1.0F, 30.0F, start_position, end_position );
}
}
}
