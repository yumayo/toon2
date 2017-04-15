#pragma once
#include "rect.h"
namespace renderer
{
class rect_edge : public rect
{
public:
    CREATE_H( rect_edge, cinder::vec2 const& size, float line_width = 1.0F);
    bool init( cinder::vec2 const& size, float line_width );
    void render( ) override;
private:
    float _line_width = 1.0F;
};
}