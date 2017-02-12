#include "collision.h"
namespace utility
{
int hit_point_polygon_2d( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 point )
{
    cinder::vec2 AB = b - a;
    cinder::vec2 BP = point - b;

    cinder::vec2 BC = c - b;
    cinder::vec2 CP = point - c;

    cinder::vec2 CA = a - c;
    cinder::vec2 AP = point - a;

    double c1 = AB.x * BP.y - AB.y * BP.x;
    double c2 = BC.x * CP.y - BC.y * CP.x;
    double c3 = CA.x * AP.y - CA.y * AP.x;

    if ( c1 >= 0.0 && c2 >= 0.0 && c3 >= 0.0 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
}