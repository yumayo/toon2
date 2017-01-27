#pragma once
#include "cinder/app/App.h"
#include "../forward.h"
namespace action
{
class action
{
    friend class action_manager;
public:
    static const int INVALID_TAG = -1;
    // t ( 0.0 ~ 1.0 )
    static float ease_liner( float t, float from, float to );

public:
    CREATE_H( action );
    bool init( );
    void setup( node_weak target, bool pause );
public:
    // ターゲットのノードが決定したら呼ばれます。
    virtual void setup( );
    // t ( 0.0 ~ 1.0 )
    virtual void step( float t );
    virtual bool is_done( );
    virtual void update( float delta );

protected:
    node_weak _target;
public:
    void set_target( node_weak const& value );
    node_weak get_target( );

protected:
    int _tag = action::INVALID_TAG;
public:
    void set_tag( int value );
    int get_tag( );

protected:
    std::string _name = "";
public:
    void set_name( std::string const& value );
    std::string get_name( );

protected:
    bool _pause = false;
public:
    void set_pause( bool value = true );
    bool get_pause( );

protected:
    size_t _hash = 0;
};
}