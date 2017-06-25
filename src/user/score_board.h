#pragma once
#include "renderer/rect.h"
#include "network.hpp"
namespace user
{
class score_board : public renderer::rect
{
public:
    CREATE_H( score_board, std::shared_ptr<node> cell_manager, cinder::vec2 size );
    bool init( std::shared_ptr<node> cell_manager, cinder::vec2 size );
private:
    std::weak_ptr<network::tcp_client> _tcp_connection;
    std::weak_ptr<node> _cell_manager;
};
}