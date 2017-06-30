#pragma once
#include "cinder/Vector.h"
struct header
{
    char name[16];
    int byte;
};
struct bullet_data
{
    int id;
    int user_id;
    cinder::vec2 position;
    cinder::vec2 direction;
    float time_remaining;
};
struct feed_data
{
    int id;
    cinder::vec2 position;
};
struct ground_data
{
    unsigned int id;
};