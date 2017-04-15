#include "node.h"
#include "cinder/gl/gl.h"
#include "utility/assert_log.h"
#include "utility/collision.h"
using namespace cinder;
using namespace utility;
CREATE_CPP( node )
{
    CREATE( node );
}
node::~node( )
{
    if ( !_name.empty( ) ) log( "Destroy node: [%s]", _name.c_str( ) );
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
bool node::touch_began( cinder::app::TouchEvent::Touch event )
{
    return false;
}
void node::touch_moved( cinder::app::TouchEvent::Touch event )
{
}
void node::touch_ended( cinder::app::TouchEvent::Touch event )
{
}
void node::touches_began( cinder::app::TouchEvent event )
{
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
bool node::_mouse_began( cinder::app::MouseEvent event )
{
    if ( !_block_schedule_event )
        for ( auto itr = _children.rbegin( ); itr != _children.rend( ); ++itr )
        {
            // 子供がモーダルオブジェクトだった場合
            if ( ( *itr )->_mouse_began( event ) )
            {
                return true;
            }
        }
    if ( _schedule_mouse_event )
    {
        if ( mouse_began( event ) )
        {
            _swallow = true;
            return true;
        }
    }
    return false;
}
bool node::_mouse_moved( cinder::app::MouseEvent event )
{
    if ( !_block_schedule_event )
        for ( auto itr = _children.rbegin( ); itr != _children.rend( ); ++itr )
        {
            if ( ( *itr )->_mouse_moved( event ) )
            {
                return true;
            }
        }
    if ( _schedule_mouse_event )
    {
        if ( _swallow )
        {
            mouse_moved( event );
            return true;
        }
    }
    return false;
}
bool node::_mouse_ended( cinder::app::MouseEvent event )
{
    if ( !_block_schedule_event )
        for ( auto itr = _children.rbegin( ); itr != _children.rend( ); ++itr )
        {
            if ( ( *itr )->_mouse_ended( event ) )
            {
                return true;
            }
        }
    if ( _schedule_mouse_event )
    {
        if ( _swallow )
        {
            mouse_ended( event );
            _swallow = false;
            return true;
        }
    }
    return false;
}
bool node::_touch_began( cinder::app::TouchEvent::Touch event )
{
    if ( !_block_schedule_event )
        for ( auto itr = _children.rbegin( ); itr != _children.rend( ); ++itr )
        {
            // 子供がモーダルオブジェクトだった場合
            if ( ( *itr )->_touch_began( event ) )
            {
                return true;
            }
        }
    if ( _schedule_touch_event )
    {
        if ( touch_began( event ) )
        {
            _swallow = true;
            return true;
        }
    }
    return false;
}
bool node::_touch_moved( cinder::app::TouchEvent::Touch event )
{
    if ( !_block_schedule_event )
        for ( auto itr = _children.rbegin( ); itr != _children.rend( ); ++itr )
        {
            if ( ( *itr )->_touch_moved( event ) )
            {
                return true;
            }
        }
    if ( _schedule_touch_event )
    {
        if ( _swallow )
        {
            touch_moved( event );
            return true;
        }
    }
    return false;
}
bool node::_touch_ended( cinder::app::TouchEvent::Touch event )
{
    if ( !_block_schedule_event )
        for ( auto itr = _children.rbegin( ); itr != _children.rend( ); ++itr )
        {
            if ( ( *itr )->_touch_ended( event ) )
            {
                return true;
            }
        }
    if ( _schedule_touch_event )
    {
        if ( _swallow )
        {
            touch_ended( event );
            _swallow = false;
            return true;
        }
    }
    return false;
}
void node::_touches_began( cinder::app::TouchEvent event )
{
    if ( !_block_schedule_event )
        for ( auto itr = _children.rbegin( ); itr != _children.rend( ); ++itr )
        {
            ( *itr )->_touches_began( event );
        }
    if ( _schedule_touches_event )
    {
        touches_began( event );
    }
}
void node::_touches_moved( cinder::app::TouchEvent event )
{
    if ( !_block_schedule_event )
        for ( auto itr = _children.rbegin( ); itr != _children.rend( ); ++itr )
        {
            ( *itr )->_touches_moved( event );
        }
    if ( _schedule_touches_event )
    {
        touches_moved( event );
    }
}
void node::_touches_ended( cinder::app::TouchEvent event )
{
    if ( !_block_schedule_event )
        for ( auto itr = _children.rbegin( ); itr != _children.rend( ); ++itr )
        {
            ( *itr )->_touches_ended( event );
        }
    if ( _schedule_touches_event )
    {
        touches_ended( event );
    }
}
void node::_update( float delta )
{
    // 途中でaddがあるため、コンテナをバックアップします。
    std::vector<std::list<std::shared_ptr<node>>::iterator> update_objects;
    if ( !_block_schedule_update )
        for ( auto itr = std::begin( _children ); itr != std::end( _children ); ++itr )
        {
            update_objects.emplace_back( itr );
        }

    for ( auto const& c : update_objects )
    {
        ( *c )->_update( delta );
    }
    _action_manager.update( delta );
    if ( _schedule_update ) update( delta );

    for ( auto const& rem : _remove_signal ) rem( );
    _remove_signal.clear( );
}
void node::_render( )
{
    if ( !_visible ) return;

    gl::pushModelView( );
    gl::translate( _position );
    gl::scale( _scale );
    gl::rotate( _rotation );
    gl::translate( -_content_size * _anchor_point );
    gl::color( _color );

    if ( utility::hit_window_aabb( shared_from_this( ) ) )
    {
        this->render( );
    }

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
void node::set_block_schedule_update( bool value )
{
    _block_schedule_update = value;
}
bool node::get_block_schedule_update( )
{
    return _block_schedule_update;
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
void node::set_schedule_touches_event( bool value )
{
    _schedule_touches_event = value;
}
bool node::get_schedule_touches_event( )
{
    return _schedule_touches_event;
}
void node::set_schedule_all( bool value )
{
    _schedule_update = value;
    _schedule_mouse_event = value;
    _schedule_touch_event = value;
    _schedule_touches_event = value;
}
void node::set_block_schedule_event( bool value )
{
    _block_schedule_event = value;
}
bool node::get_block_schedule_event( )
{
    return _block_schedule_event;
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
void node::set_opacity( float alpha )
{
    _color.a = alpha;
}
float node::get_opacity( )
{
    return _color.a;
}
std::list<std::shared_ptr<node>>& node::get_children( )
{
    return _children;
}
void node::set_parent( std::shared_ptr<node> const& value )
{
    std::weak_ptr<node> prev_parent = _parent;
    value->add_child( shared_from_this( ) );

    if ( prev_parent.lock( ) )
    {
        prev_parent.lock( )->remove_child( shared_from_this( ) );
    }
}
std::shared_ptr<node> node::get_parent( )
{
    return _parent.lock( );
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
    _children.sort( [ ] ( std::shared_ptr<node>& a, std::shared_ptr<node>& b )
    {
        return a->_order < b->_order;
    } );
}
int node::get_order( )
{
    return _order;
}
void node::set_name( std::string const& value )
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
void node::add_child( std::shared_ptr<node> const& value )
{
    value->_parent = shared_from_this( );
    _children.emplace_back( std::move( value ) );
}
std::shared_ptr<node> node::get_child_by_name( std::string const & name )
{
    assert_log( !name.empty( ), "無効な名前です。", return nullptr );

    std::hash<std::string> h;
    size_t hash = h( name );

    auto itr = std::find_if( std::begin( _children ), std::end( _children ), [ this, hash, name ] ( std::shared_ptr<node>& child )
    {
        return child->_hash == hash && child->_name.compare( name ) == 0;
    } );

    if ( itr != std::end( _children ) )
    {
        return *itr;
    }
    return nullptr;
}
std::shared_ptr<node> node::get_child_by_tag( int tag )
{
    assert_log( tag == node::INVALID_TAG, "無効なタグです。", return nullptr );

    auto itr = std::find_if( std::begin( _children ), std::end( _children ), [ this, tag ] ( std::shared_ptr<node>& n )
    {
        return n->_tag == tag;
    } );

    if ( itr != std::end( _children ) )
    {
        return *itr;
    }
    return nullptr;
}
void node::remove_child( std::shared_ptr<node> const& child )
{
    if ( _children.empty( ) ) return;

    auto erase = std::remove_if( std::begin( _children ), std::end( _children ), [ this, child ] ( std::shared_ptr<node>& n )
    {
        return n == child;
    } );

    _children.erase( erase, std::end( _children ) );
}
void node::remove_child_by_name( std::string const & name )
{
    assert_log( !name.empty( ), "無効な名前です。", return );

    auto child = this->get_child_by_name( name );

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
    assert_log( tag == node::INVALID_TAG, "無効なタグです。", return );

    auto child = this->get_child_by_tag( tag );

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
    _remove_signal.emplace_back( [ this ]
    {
        _children.clear( );
    } );
}
void node::remove_from_parent( )
{
    if ( !_own_removing )
    {
        _own_removing = true;
        if ( _parent.lock( ) )
        {
            _parent.lock( )->_remove_signal.emplace_back( [ this ]
            {
                _parent.lock( )->remove_child( shared_from_this( ) );
            } );
        }
    }
}
void node::remove_from_parent_user_function( std::function<void( )> remove_user_function )
{
    if ( !_own_removing )
    {
        _own_removing = true;
        if ( _parent.lock( ) )
        {
            _parent.lock( )->_remove_signal.emplace_back( [ this, remove_user_function ]
            {
                _parent.lock( )->remove_child( shared_from_this( ) );
                if ( remove_user_function ) remove_user_function( );
            } );
        }
    }
}
std::shared_ptr<node> node::get_root( )
{
    return _get_root( );
}

std::shared_ptr<node> node::_get_root( )
{
    if ( _parent.lock( ) )
    {
        return _parent.lock( )->_get_root( );
    }
    else
    {
        return shared_from_this( );
    }
}

void node::run_action( std::shared_ptr<action::action> const & action )
{
    _action_manager.add_action( action, shared_from_this( ), !_running );
    action->setup( );
}

std::shared_ptr<action::action> node::get_action_by_name( std::string const & name )
{
    return _action_manager.get_action_by_name( name );
}

std::shared_ptr<action::action> node::get_action_by_tag( int tag )
{
    return _action_manager.get_action_by_tag( tag );
}

void node::remove_all_actions( )
{
    _action_manager.remove_all_actions( );
}

void node::remove_action( std::shared_ptr<action::action> const & action )
{
    _action_manager.remove_action( action );
}

void node::remove_action_by_tag( int tag )
{
    _action_manager.remove_action_by_tag( tag );
}

void node::remove_action_by_name( std::string const & name )
{
    _action_manager.remove_action_by_name( name );
}

bool node::is_running_action( )
{
    return _action_manager.is_running( );
}

cinder::mat3 node::get_world_matrix( )
{
    std::vector<mat3> mats;
    auto p = _parent;
    while ( p.lock( ) )
    {
        auto m = translate( mat3( ), p.lock( )->_position );
        m = scale( m, p.lock( )->_scale );
        m = rotate( m, p.lock( )->_rotation );
        m = translate( m, -p.lock( )->_content_size * p.lock( )->_anchor_point );
        m = translate( m, p.lock( )->_content_size * p.lock( )->_pivot );
        mats.emplace_back( std::move( m ) );
        p = p.lock( )->_parent;
    }

    mat3 result;
    for ( auto itr = mats.rbegin( ); itr != mats.rend( ); ++itr )
    {
        result *= *itr;
    }

    return result;
}