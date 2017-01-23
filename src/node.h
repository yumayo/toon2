#pragma once
#include "cinder/app/App.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <vector>
#include <string>
#include <memory>
#define CREATE(__TYPE__)\
template <class... Args>\
static std::shared_ptr<__TYPE__> create( Args... args )\
{\
    auto ret = std::make_shared<__TYPE__>( );\
    if ( ret && ret->init( args... ) )\
        ;\
    else\
    {\
        ret.reset( );\
    }\
    return std::move( ret ); \
}
class node
{
public:
    CREATE( node );
public:
    virtual ~node( );
    virtual bool init( );
    virtual void clean( );
    virtual void mouse_began( cinder::app::MouseEvent event );
    virtual void mouse_moved( cinder::app::MouseEvent event );
    virtual void mouse_ended( cinder::app::MouseEvent event );
    virtual void touches_began( cinder::app::TouchEvent event );
    virtual void touches_moved( cinder::app::TouchEvent event );
    virtual void touches_ended( cinder::app::TouchEvent event );
    virtual void update( float delta );
    virtual void draw( );

protected:
    bool _schedule_update = false;
public:
    void set_schedule_update( bool value = true );
    bool get_schedule_update( );

protected:
    bool _schedule_mouse_event = false;
public:
    void set_schedule_mouse_event( bool value = true );
    bool get_schedule_mouse_event( );

protected:
    bool _schedule_touch_event = false;
public:
    void set_schedule_touch_event( bool value = true );
    bool get_schedule_touch_event( );

protected:
    cinder::vec2 _position = cinder::vec2( 0 );
public:
    void set_position( cinder::vec2 value );
    cinder::vec2 get_position( );

protected:
    cinder::vec2 _scale = cinder::vec2( 0 );
public:
    void set_scale( cinder::vec2 value );
    cinder::vec2 get_scale( );

protected:
    cinder::vec2 _content_size = cinder::vec2( 0 );
public:
    void set_content_size( cinder::vec2 value );
    cinder::vec2 get_content_size( );

protected:
    float _rotation = 0.0F;
public:
    void set_rotation( float value );
    float get_rotation( );

protected:
    cinder::vec2 _anchor_point = cinder::vec2( 0 );
public:
    void set_anchor_point( cinder::vec2 value );
    cinder::vec2 get_anchor_point( );

protected:
    cinder::vec2 _pivot = cinder::vec2( 0 );
public:
    void set_pivot( cinder::vec2 value );
    cinder::vec2 get_pivot( );

protected:
    cinder::ColorA _color = cinder::ColorA::black( );
public:
    void set_color( cinder::ColorA value );
    cinder::ColorA get_color( );

protected:
    std::vector<std::shared_ptr<node>> _children;
public:
    void add_child( std::shared_ptr<node> const& value );
    std::vector<std::shared_ptr<node>> const& get_children( );

protected:
    node* _parent = nullptr;
public:
    node* get_parent( );

protected:
    size_t _tag = 0;
public:
    void set_tag( size_t value );
    size_t get_tag( );

protected:
    int _order = 0;
public:
    void set_order( int value );
    int get_order( );

protected:
    std::string _name = u8"";
public:
    void set_name( std::string value );
    std::string get_name( );

protected:
    size_t _hash = 0;

protected:
    bool _running = false;
public:
    void set_running( bool value );
    bool get_running( );

protected:
    bool _visible = false;
public:
    void set_visible( bool value );
    bool get_visible( );

};