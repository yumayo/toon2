#pragma once
#include "cinder/Vector.h"
namespace user
{
struct toon_data
{
    cinder::vec2 position;
    float radius = 0.0F;
};
class toon_packet
{
    // 5フレーム前までのデータを詰めます。
    toon_data data[5];
};
}