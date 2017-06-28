#pragma once
#include "node.h"
#include "network.hpp"
#include "jsoncpp/json.h"
namespace user
{
// �T�[�o�[�ƕ��􂵂��זE�Ɠ������Ƃ�B
// ���������ɁA�t�B�[���h�̏����T�[�o�[����󂯎��B
// �K�v�ȏ��́A
// id (division_cell��"id"�ł��B"tag"�Ƃ��Ĉ����܂��B)
// position
// radius
// color ("color"��"skin_name"��"player_id"���琄�����܂��B)
// skin_name

// �ŏI�I�ɂ�
// int id
// vec2 position
// float radius
// int player_id

// ����G�T�̃C�[�W���O�r���̍זE������̂�
// int id
// vec2 position
// vec2 direction
// float time
// float radius
// int player_id

class division_cell_manager : public node
{
public:
    CREATE_H( division_cell_manager, std::weak_ptr<node> cell_manager, std::vector<Json::Value> const& division_cells_buffer );
    bool init( std::weak_ptr<node> cell_manager, std::vector<Json::Value> const& division_cells_buffer );
    void update( float delta ) final override;
    void on_feed_captured( int tag );
protected:
    int _number_of_captured_division_cell = 0;
    Json::Value _captured_division_cell_data;
    std::weak_ptr<node> _cell_manager;
    std::weak_ptr<network::tcp_client> _tcp_connection;
};
}