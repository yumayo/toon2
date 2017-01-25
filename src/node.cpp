#include "node.h"
#include "cinder/gl/gl.h"
#include "utility.h"
using namespace cinder;
CREATE_CPP( node )
{
    CREATE( node );
}
node::~node( )
{
    log( "~node(): [%s]", _name.c_str( ) );
}
bool node::mouse_began( cinder::app::MouseEvent event )
{
    return false;
}
void node::mouse_moved( cinder::app::MouseEvent event )
{
}
void node::mouse_ended( cinder::app::MouseEvent event )
{
}
bool node::touches_began( cinder::app::TouchEvent event )
{
    return false;
}
void node::touches_moved( cinder::app::TouchEvent event )
{
}
void node::touches_ended( cinder::app::TouchEvent event )
{
}
void node::update( float delta )
{
}
void node::render( )
{
}
void node::_mouse_began( cinder::app::MouseEvent event )
{
    for ( auto const& c : _children )
    {
        c->_mouse_began( event );
    }

    if ( _schedule_mouse_event && !get_root( )->_event_target )
    {
        if ( mouse_began( event ) )
        {
            get_root( )->_event_target = this;
        }
    }
}
void node::_mouse_moved( cinder::app::MouseEvent event )
{
    // ルートの時点でイベントがあったらその処理をして、イベントを終了します。
    if ( _event_target )
    {
        _event_target->mouse_moved( event );
        return;
    }

    // 上を通過した時点で全てのオブジェクトを通せるので、再帰的に処理をします。
    for ( auto const& c : _children )
    {
        c->_mouse_moved( event );
    }
    if ( _schedule_mouse_event ) mouse_moved( event );
}
void node::_mouse_ended( cinder::app::MouseEvent event )
{
    // ルートの時点でイベントがあったらその処理をして、イベントを終了します。
    if ( _event_target )
    {
        _event_target->mouse_ended( event );
        _event_target = nullptr;
        return;
    }

    // 上を通過した時点で全てのオブジェクトを通せるので、再帰的に処理をします。
    for ( auto const& c : _children )
    {
        c->_mouse_ended( event );
    }
    if ( _schedule_mouse_event ) mouse_ended( event );
}
void node::_touches_began( cinder::app::TouchEvent event )
{
    for ( auto const& c : _children )
    {
        c->_touches_began( event );
    }

    if ( _schedule_touch_event && !get_root( )->_event_target )
    {
        if ( touches_began( event ) )
        {
            get_root( )->_event_target = this;
        }
    }
}
void node::_touches_moved( cinder::app::TouchEvent event )
{
    // ルートの時点でイベントがあったらその処理をして、イベントを終了します。
    if ( _event_target )
    {
        _event_target->touches_moved( event );
        return;
    }

    // 上を通過した時点で全てのオブジェクトを通せるので、再帰的に処理をします。
    for ( auto const& c : _children )
    {
        c->_touches_moved( event );
    }
    if ( _schedule_mouse_event ) touches_moved( event );
}
void node::_touches_ended( cinder::app::TouchEvent event )
{
    // ルートの時点でイベントがあったらその処理をして、イベントを終了します。
    if ( _event_target )
    {
        _event_target->touches_ended( event );
        _event_target = nullptr;
        return;
    }

    // 上を通過した時点で全てのオブジェクトを通せるので、再帰的に処理をします。
    for ( auto const& c : _children )
    {
        c->_touches_ended( event );
    }
    if ( _schedule_mouse_event ) touches_ended( event );
}
void node::_update( float delta )
{
    for ( auto const& c : _children )
    {
        if ( c->_schedule_update ) c->_update( delta );
    }
    update( delta );
}
void node::_render( )
{
    gl::pushModelView( );
    gl::translate( _position );
    gl::scale( _scale );
    gl::rotate( _rotation );
    gl::translate( -_content_size * _anchor_point );
    gl::color( _color );
    this->render( );
    gl::translate( _content_size * _pivot );
    for ( auto const& c : _children )
    {
        c->_render( );
    }
    gl::popModelView( );
}
bool node::init( )
{
    return true;
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
    _content_size = value;
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
std::vector<node_ref> const & node::get_children( )
{
    return _children;
}
void node::set_parent( node* value )
{
    _parent = value;
}
node* node::get_parent( )
{
    return _parent;
}
void node::set_tag( int value )
{
    _tag = value;
}
int node::get_tag( )
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

    std::hash<std::string> h;
    _hash = h( value );
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
void node::add_child( node_ref value )
{
    value->_parent = this;
    _children.emplace_back( value );
}
node_ref node::get_child_by_name( std::string const & name )
{
    ASSERT( !name.empty( ), "無効な名前です。" );

    std::hash<std::string> h;
    size_t hash = h( name );

    auto itr = std::find_if( std::begin( _children ), std::end( _children ), [ this, hash, name ] ( node_ref& child )
    {
        return child->_hash == hash && child->_name.compare( name ) == 0;
    } );

    if ( itr != std::end( _children ) )
    {
        return *itr;
    }
    return node_ref( );
}
node_ref node::get_child_by_tag( int tag )
{
    ASSERT( tag != node::INVALID_TAG, "無効なタグです。" );

    auto itr = std::find_if( std::begin( _children ), std::end( _children ), [ this, tag ] ( node_ref& n )
    {
        return n->_tag == tag;
    } );

    if ( itr != std::end( _children ) )
    {
        return *itr;
    }
    return node_ref( );
}
void node::remove_child( node_ref child )
{
    if ( _children.empty( ) ) return;

    auto itr = std::find_if( std::begin( _children ), std::end( _children ), [ this, child ] ( node_ref& n )
    {
        return n == child;
    } );
    _children.erase( itr );
}
void node::remove_child_by_name( std::string const & name )
{
    ASSERT( !name.empty( ), "無効な名前です。" );

    node_ref child = this->get_child_by_name( name );

    if ( child )
    {
        this->remove_child( child );
    }
    else
    {
        log( "remove_child_by_name(name = %s): 子供が見つかりませんでした。", name.c_str( ) );
    }
}
void node::remove_child_by_tag( int tag )
{
    ASSERT( tag != node::INVALID_TAG, "Invalid tag" );

    node_ref child = this->get_child_by_tag( tag );

    if ( child )
    {
        this->remove_child( child );
    }
    else
    {
        log( "remove_child_by_tag(tag = %d): 子供が見つかりませんでした。", tag );
    }
}
void node::remove_all_children( )
{
    _children.clear( );
}
void node::remove_from_parent( )
{
    if ( _parent )
    {
        _parent->remove_child( shared_from_this( ) );
    }
}
node* node::get_root( )
{
    return _get_root( );
}

node* node::_get_root( )
{
    if ( _parent )
    {
        return _parent->_get_root( );
    }
    else
    {
        return this;
    }
}

cinder::mat3 node::get_world_matrix( )
{
    std::vector<mat3> mats;
    auto p = _parent;
    while ( p )
    {
        auto m = translate( mat3( ), p->_position );
        m = scale( m, p->_scale );
        m = rotate( m, p->_rotation );
        m = translate( m, -p->_content_size * p->_anchor_point );
        m = translate( m, p->_content_size * p->_pivot );
        mats.emplace_back( std::move( m ) );
        p = p->_parent;
    }

    mat3 result;
    for ( auto itr = mats.rbegin( ); itr != mats.rend( ); ++itr )
    {
        result *= *itr;
    }

    return result;
}
