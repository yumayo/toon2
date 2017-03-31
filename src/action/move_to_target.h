#pragma once
#include "finite_time_action.h"
#include "cinder/Vector.h"
namespace action
{
class move_to_target : public finite_time_action
{
public:
    CREATE_H( move_to_target, float duration, std::shared_ptr<node> const& target );
    bool init( float duration, std::shared_ptr<node> const& target );
private:
    void setup( ) override;
    void step( float t ) override;
protected:
    cinder::vec2 _start_position;
    std::weak_ptr<node> _new_target;
};
}