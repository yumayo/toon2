#pragma once
#include "finite_time_action.h"
#include "cinder/Vector.h"
namespace action
{
class move_to : public finite_time_action
{
public:
    CREATE_H( move_to, float duration, cinder::vec2 position );
    bool init( float duration, cinder::vec2 position );
private:
    void setup( ) override;
    void step( float t ) override;
protected:
    cinder::vec2 _start_position;
    cinder::vec2 _position;
};
}