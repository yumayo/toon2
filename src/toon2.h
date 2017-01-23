#pragma once
#include "node.h"
#include "cinder/Camera.h"
class toon2 : public node
{
public:
    CREATE( toon2 );
    bool init( ) override;
    void update( float delta ) override;
    void draw( ) override;
private:
    cinder::CameraPersp camera;
    float rotate = 0.0F;
};