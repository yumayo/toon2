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
    log( "Destroy node: [%s]", _name.c_str( ) );
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
    // 途中でaddがあるため、iteratorをバックアップします。
    std::vector<std::vector<std::shared_ptr<node>>::iterator> update_iterators;
    for ( auto itr = std::begin( _children ); itr != std::end( _children ); ++itr )
    {
        update_iterators.emplace_back( itr );
    }

    for ( auto const& c : update_iterators )
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

    // 軽減処理
    auto mat = get_world_matrix( );
    auto obj = mat;
    obj = translate( obj, _position );
    obj = scale( obj, _scale );
    obj = rotate( obj, _rotation );
    auto slide_size = -_content_size * _anchor_point;
    auto start = vec2( 100, 100 );
    auto size = vec2( app::getWindowSize( ) ) - start;
    vec2 e = { start.x, start.y }, f = { size.x, start.y }, g = { size.x, size.y }, h = { start.x, size.y };
    auto ma = translate( obj, slide_size + vec2( 0.0F, 0.0F ) );
    auto a = vec2( ma[2][0], ma[2][1] );
    auto mb = translate( obj, slide_size + vec2( _content_size.x, 0.0F ) );
    auto b = vec2( mb[2][0], mb[2][1] );
    auto mc = translate( obj, slide_size + vec2( _content_size.x, _content_size.y ) );
    auto c = vec2( mc[2][0], mc[2][1] );
    auto md = translate( obj, slide_size + vec2( 0.0F, _content_size.y ) );
    auto d = vec2( md[2][0], md[2][1] );

    if ( utility::hit_point_plane_2d( a, b, c, d, e ) ||
         utility::hit_point_plane_2d( a, b, c, d, f ) ||
         utility::hit_point_plane_2d( a, b, c, d, g ) ||
         utility::hit_point_plane_2d( a, b, c, d, h ) ||
         utility::hit_point_plane_2d( e, f, g, h, a ) ||
         utility::hit_point_plane_2d( e, f, g, h, b ) ||
         utility::hit_point_plane_2d( e, f, g, h, c ) ||
         utility::hit_point_plane_2d( e, f, g, h, d ) ||
         utility::hit_segment( d, a, h, e ) )
    {
        // hit
        this->render( );
    }
    // 画面外にオブジェクトが出たら描画しません。
    else
        auto name = _name;


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
void node::set_schedule_touches_event( bool value )
{
    _schedule_touches_event = value;
}
bool node::get_schedule_touches_event( )
{
    return _schedule_touches_event;
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
std::vector<std::shared_ptr<node>> const & node::get_children( )
{
    return _children;
}
void node::set_parent( std::shared_ptr<node> const& value )
{
    _parent = value;
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
    _children.clear( );
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

#define l_class node
#include "lua_define.h"
LUA_SETUP_CPP( l_class )
{
    l_new( node
           , l_set( create )
           , l_prop( schedule_update )
           , l_prop( schedule_mouse_event )
           , l_prop( schedule_touch_event )
           , l_prop( schedule_touches_event )
           , l_prop( position )
           , l_prop( scale )
           , l_prop( content_size )
           , l_prop( rotation )
           , l_prop( anchor_point )
           , l_prop( pivot )
           , l_prop( color )
           , l_prop( opacity )
           , l_readonly( children )
           , l_prop( parent )
           , l_prop( tag )
           , l_prop( order )
           , l_prop( name )
           , l_prop( running )
           , l_prop( visible )
           , l_set( add_child )
           , l_set( get_child_by_name )
           , l_set( get_child_by_tag )
           , l_set( remove_child )
           , l_set( remove_child_by_name )
           , l_set( remove_child_by_tag )
           , l_set( remove_all_children )
           , l_set( remove_from_parent )
           , l_set( remove_from_parent_user_function )
           , l_set( get_root )
           , l_set( run_action )
           , l_set( get_world_matrix )
    );
}
#include "lua_undef.h"