#pragma once
#include <treelike/node.h>
namespace user
{
class bullet : public treelike::node
{
public:
    CREATE_H( bullet, int bullet_id, float radius, cinder::ColorA color );
    bool init( int bullet_id, float radius, cinder::ColorA color );
public:
    float get_radius( ) const;
    void hit( );
    bool is_hit( );
private:
    float _radius;
    bool _is_hit = false;
};
}