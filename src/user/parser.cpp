#include "parser.h"
#include "player.h"
#include "analog_axis.h"
#include <treelike/user_default.h>
using namespace cinder;
using namespace treelike;
namespace user
{
namespace parser
{
bool bullet_straight( Json::Value * root, softptr<treelike::node> n_player, softptr<treelike::node> n_analog_axis )
{
    auto player = n_player.dynamicptr<user::player>( );
    auto analog_axis = n_analog_axis.dynamicptr<user::analog_axis>( );

    // ¬‚³‚¢ê‡‚Í‘Å‚Ä‚È‚¢B
    if ( player->get_target_radius( ) < user_default::get_instans( )->get_root( )["system"]["min_radius"].asFloat( ) + 4.0F ) return false;

    auto start_position = player->get_position( ) + player->get_radius( ) * analog_axis->get_last_normalized_axis( );
    auto end_position = start_position + 800.0F * analog_axis->get_last_normalized_axis( );

    player->scale_action( -4.0F );
    Json::Value& data = *root;
    data["user_id"] = player->get_tag( );
    data["time"] = 2.0F;
    data["start_position"][0] = start_position.x;
    data["start_position"][1] = start_position.y;
    data["end_position"][0] = end_position.x;
    data["end_position"][1] = end_position.y;

    return true;
}
}
}
