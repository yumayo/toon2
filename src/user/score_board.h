#pragma once
#include <treelike/renderer/rect.h>
#include <treelike/network.hpp>
namespace user
{
class score_board : public treelike::renderer::rect
{
public:
    CREATE_H( score_board, std::shared_ptr<treelike::node> cell_manager, cinder::vec2 size );
    bool init( std::shared_ptr<treelike::node> cell_manager, cinder::vec2 size );
private:
    softptr<treelike::network::tcp_client> _tcp_connection;
    softptr<treelike::node> _cell_manager;
};
}