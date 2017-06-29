#pragma once
#include "captured_object.h"
namespace user
{
class feed : public captured_object
{
public:
    CREATE_H( feed, int tag, cinder::vec2 position );
    bool init( int tag, cinder::vec2 position );
public:
    float get_radius( );
    void on_captured( std::weak_ptr<node> other ) final override;
protected:
    float _score = 0.0F;
    float _radius = 0.0F;
};
}