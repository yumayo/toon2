#pragma once
#include "cinder/Vector.h"
#include <memory>
#include "node.h"
namespace utility
{
// http://www.sousakuba.com/Programming/gs_hittest_point_triangle.html
bool hit_point_polygon_2d( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 point );

// http://marupeke296.com/COL_2D_No4_SquareToSquare.html
bool hit_point_plane_2d( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d, cinder::vec2 point );
bool hit_point_plane_2d( std::shared_ptr<node> const& object, cinder::vec2 point );
bool hit_point_plane_2d_default_size( std::shared_ptr<node> const& object, cinder::vec2 point );

// https://gist.github.com/yoshiki/7702066
bool hit_segment( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 );
cinder::vec2 get_hit_segment_intersection( cinder::vec2 s1, cinder::vec2 e1, cinder::vec2 s2, cinder::vec2 e2 );

// http://marupeke296.com/COL_2D_No4_SquareToSquare.html
bool hit_quad_quad( cinder::vec2 a1, cinder::vec2 b1, cinder::vec2 c1, cinder::vec2 d1,
                    cinder::vec2 a2, cinder::vec2 b2, cinder::vec2 c2, cinder::vec2 d2 );
bool hit_window( std::shared_ptr<node>const& object );
bool hit_window_aabb( std::shared_ptr<node> const& object );

float determinant_2d( cinder::vec2 a, cinder::vec2 b );

std::pair<cinder::vec2, cinder::vec2> create_aabb( cinder::vec2 a, cinder::vec2 b, cinder::vec2 c, cinder::vec2 d );
}