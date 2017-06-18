#pragma once
#include "renderer/rect.h"
#include "network.hpp"
namespace user
{
class score_board : public renderer::rect
{
public:
    CREATE_H( score_board, cinder::vec2 size );
    bool init( cinder::vec2 size );
private:
    std::shared_ptr<node> create_score( int rank, int id, int score );
    std::weak_ptr<network::tcp_client> _tcp_connection;
};
}