#include "node.h"
#include "boost/optional.hpp"
using namespace cinder;
node::~node( )
{
    app::console( ) << "~node() : [" << _name << "]" << std::endl;
}
bool node::init( )
{
    return true;
}
void node::clean( )
{
    for ( auto const& c : _children )
    {
        c->clean( );
    }
}
void node::mouse_began( cinder::app::MouseEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->_schedule_mouse_event ) c->mouse_began( event );
    }
}
void node::mouse_moved( cinder::app::MouseEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->_schedule_mouse_event ) c->mouse_moved( event );
    }
}
void node::mouse_ended( cinder::app::MouseEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->_schedule_mouse_event ) c->mouse_ended( event );
    }
}
void node::touches_began( cinder::app::TouchEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->_schedule_touch_event ) c->touches_began( event );
    }
}
void node::touches_moved( cinder::app::TouchEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->_schedule_touch_event ) c->touches_moved( event );
    }
}
void node::touches_ended( cinder::app::TouchEvent event )
{
    for ( auto const& c : _children )
    {
        if ( c->_schedule_touch_event ) c->touches_ended( event );
    }
}
void node::update( float delta )
{
    for ( auto const& c : _children )
    {
        if ( c->_schedule_update ) c->update( delta );
    }
}
void node::draw( )
{
    for ( auto const& c : _children )
    {
        c->draw( );
    }
}
void node::set_schedule_update( bool value )
{
    _schedule_update = value;
}
bool node::get_schedule_update( )
{
    return _schedule_update;
}
void node::set_schedule_mouse_event( bool value )
{
    _schedule_mouse_event = value;
}
bool node::get_schedule_mouse_event( )
{
    return _schedule_mouse_event;
}
void node::set_schedule_touch_event( bool value )
{
    _schedule_touch_event = value;
}
bool node::get_schedule_touch_event( )
{
    return _schedule_touch_event;
}
void node::set_position( cinder::vec2 value )
{
    _position = value;
}
cinder::vec2 node::get_position( )
{
    return _position;
}
void node::set_scale( cinder::vec2 value )
{
    _scale = value;
}
cinder::vec2 node::get_scale( )
{
    return _scale;
}
void node::set_content_size( cinder::vec2 value )
{
}
cinder::vec2 node::get_content_size( )
{
    return _content_size;
}
void node::set_rotation( float value )
{
    _rotation = value;
}
float node::get_rotation( )
{
    return _rotation;
}
void node::set_anchor_point( cinder::vec2 value )
{
    _anchor_point = value;
}
cinder::vec2 node::get_anchor_point( )
{
    return _anchor_point;
}
void node::set_pivot( cinder::vec2 value )
{
    _pivot = value;
}
cinder::vec2 node::get_pivot( )
{
    return _pivot;
}
void node::set_color( cinder::ColorA value )
{
    _color = value;
}
cinder::ColorA node::get_color( )
{
    return _color;
}
void node::add_child( std::shared_ptr<node> const& value )
{
    _children.emplace_back( value );
}
std::vector<std::shared_ptr<node>> const & node::get_children( )
{
    return _children;
}
node * node::get_parent( )
{
    return _parent;
}
void node::set_tag( size_t value )
{
    _tag = value;
}
size_t node::get_tag( )
{
    return _tag;
}
void node::set_order( int value )
{
    _order = value;
}
int node::get_order( )
{
    return _order;
}
void node::set_name( std::string value )
{
    _name = value;
}
std::string node::get_name( )
{
    return _name;
}
void node::set_running( bool value )
{
    _running = value;
}
bool node::get_running( )
{
    return _running;
}
void node::set_visible( bool value )
{
    _visible = value;
}
bool node::get_visible( )
{
    return _visible;
}