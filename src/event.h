#pragma once
#include "cinder/Vector.h"
class touch_event
{
private:
    int _id;
    cinder::vec2 _touch_position;
    cinder::vec2 _position;
    cinder::vec2 _prev_position;
};