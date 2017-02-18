#include "collision.h"
using namespace cinder;
namespace utility
{
int hit_point_polygon_2d( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 point )
{
    vec2 AB = b - a;
    vec2 BP = point - b;

    vec2 BC = c - b;
    vec2 CP = point - c;

    vec2 CA = a - c;
    vec2 AP = point - a;

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

int hit_point_plane_2d( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d, cinder::vec2 point )
{
    vec2 AB = b - a;
    vec2 BP = point - b;

    vec2 BC = c - b;
    vec2 CP = point - c;

    vec2 CD = d - c;
    vec2 DP = point - d;

    vec2 DA = a - d;
    vec2 AP = point - a;

    double c1 = AB.x * BP.y - AB.y * BP.x;
    double c2 = BC.x * CP.y - BC.y * CP.x;
    double c3 = CD.x * DP.y - CD.y * DP.x;
    double c4 = DA.x * AP.y - DA.y * AP.x;

    if ( c1 >= 0.0 && c2 >= 0.0 && c3 >= 0.0 && c4 >= 0.0 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
bool hit_segment( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 )
{
    auto determinant = [ ] ( vec2 a, vec2 b )
    {
        return a.x * b.y - a.y * b.x;
    };

    const vec2 sub_start = s1 - s2;
    const vec2 sub_seg1 = e1 - s1;
    const vec2 sub_seg2 = e2 - s2;

    const double seg1_t = determinant( sub_seg2, sub_start ) / determinant( sub_seg1, sub_seg2 );
    if ( seg1_t < 0 || 1 < seg1_t ) return false;

    const double seg2_t = determinant( sub_seg1, sub_start ) / determinant( sub_seg1, sub_seg2 );
    if ( seg2_t < 0 || 1 < seg2_t ) return false;

    return true;
}
vec2 get_hit_segment_intersection( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 )
{
    auto determinant = [ ] ( vec2 a, vec2 b )
    {
        return a.x * b.y - a.y * b.x;
    };

    const vec2 sub_start = s1 - s2;
    const vec2 sub_seg1 = e1 - s1;
    const vec2 sub_seg2 = e2 - s2;

    const double seg1_t = determinant( sub_seg2, sub_start ) / determinant( sub_seg1, sub_seg2 );
    if ( seg1_t < 0 || 1 < seg1_t ) return vec2( 0 );

    const double seg2_t = determinant( sub_seg1, sub_start ) / determinant( sub_seg1, sub_seg2 );
    if ( seg2_t < 0 || 1 < seg2_t ) return vec2( 0 );

    return vec2( sub_seg1.x * seg1_t + s1.x, sub_seg1.y * seg1_t + s1.y );
}
}