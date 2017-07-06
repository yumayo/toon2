#pragma once
#include <treelike/node.h>
#include <treelike/network.hpp>
#include "jsoncpp/json.h"
namespace user
{
// サーバーと分裂した細胞と同期をとる。
// 初期化時に、フィールドの情報をサーバーから受け取る。
// 必要な情報は、
// id (division_cellの"id"です。"tag"として扱います。)
// position
// radius
// color ("color"と"skin_name"は"player_id"から推測します。)
// skin_name

// 最終的には
// int id
// vec2 position
// float radius
// int player_id

// いやエサのイージング途中の細胞もあるので
// int id
// vec2 position
// vec2 direction
// float time
// float radius
// int player_id

class division_cell_manager : public treelike::node
{
public:
    CREATE_H( division_cell_manager, softptr<treelike::node> cell_manager, std::vector<Json::Value> const& division_cells_buffer );
    bool init( softptr<treelike::node> cell_manager, std::vector<Json::Value> const& division_cells_buffer );
    void update( float delta ) final override;
    void on_feed_captured( int tag );
protected:
    int _number_of_captured_division_cell = 0;
    Json::Value _captured_division_cell_data;
    softptr<treelike::node> _cell_manager;
    softptr<treelike::network::tcp_client> _tcp_connection;
};
}