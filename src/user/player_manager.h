#pragma once
#include "../node.h"
#include "player.h"
#include "../network/udp_object.h"
namespace user
{
class player_manager : public node
{
public:
    CREATE_H( player_manager );
    bool init( );
    void update( float delta ) override;
public:
    std::shared_ptr<player> get_player( );
    void packet_loss_completion( std::function<void(cinder::vec2 position )> on_packet_loss );
protected:
    std::weak_ptr<player> _player;
    std::weak_ptr<player> _enemy;
    std::weak_ptr<network::udp_object> _udp;
};
}