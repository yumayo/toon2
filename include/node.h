#pragma once
#include "cinder/app/App.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <vector>
#include <string>
#include "forward.h"
#include "action/action_manager.h"

class node : public std::enable_shared_from_this<node>
{
    friend class app_delegate;
public:
    static const int INVALID_TAG = -1;

public:
    CREATE_H( node );
public:
    virtual ~node( );
public:
    virtual bool mouse_began( cinder::app::MouseEvent event );
    virtual void mouse_moved( cinder::app::MouseEvent event );
    virtual void mouse_ended( cinder::app::MouseEvent event );
    virtual bool touch_began( cinder::app::TouchEvent::Touch event );
    virtual void touch_moved( cinder::app::TouchEvent::Touch event );
    virtual void touch_ended( cinder::app::TouchEvent::Touch event );
    virtual void touches_began( cinder::app::TouchEvent event );
    virtual void touches_moved( cinder::app::TouchEvent event );
    virtual void touches_ended( cinder::app::TouchEvent event );
    virtual void update( float delta );
    virtual void render( );
private:
    // à»â∫ÇÃä÷êîÇ≈ÉmÅ[Éhä‘ÇâÒÇµÇ‹Ç∑ÅB
    bool _mouse_began( cinder::app::MouseEvent event );
    bool _mouse_moved( cinder::app::MouseEvent event );
    bool _mouse_ended( cinder::app::MouseEvent event );
    bool _touch_began( cinder::app::TouchEvent::Touch event );
    bool _touch_moved( cinder::app::TouchEvent::Touch event );
    bool _touch_ended( cinder::app::TouchEvent::Touch event );
    void _touches_began( cinder::app::TouchEvent event );
    void _touches_moved( cinder::app::TouchEvent event );
    void _touches_ended( cinder::app::TouchEvent event );
    void _update( float delta );
    void _render( );

protected:
    bool init( );

protected:
    bool _schedule_update = false;
public:
    virtual void set_schedule_update( bool value = true );
    bool get_schedule_update( );

protected:
    bool _block_schedule_update = false;
public:
    virtual void set_block_schedule_update( bool value = true );
    bool get_block_schedule_update( );

protected:
    bool _schedule_mouse_event = false;
public:
    virtual void set_schedule_mouse_event( bool value = true );
    bool get_schedule_mouse_event( );

protected:
    bool _schedule_touch_event = false;
public:
    virtual void set_schedule_touch_event( bool value = true );
    bool get_schedule_touch_event( );

protected:
    bool _schedule_touches_event = false;
public:
    virtual void set_schedule_touches_event( bool value = true );
    bool get_schedule_touches_event( );

public:
    virtual void set_schedule_all( bool value = true );

protected:
    bool _block_schedule_event = false;
public:
    virtual void set_block_schedule_event( bool value = true );
    bool get_block_schedule_event( );

protected:
    cinder::vec2 _position = cinder::vec2( 0 );
public:
    virtual void set_position( cinder::vec2 value );
    cinder::vec2 get_position( );

protected:
    cinder::vec2 _scale = cinder::vec2( 1 );
public:
    virtual void set_scale( cinder::vec2 value );
    cinder::vec2 get_scale( );

protected:
    cinder::vec2 _content_size = cinder::vec2( 0 );
public:
    virtual void set_content_size( cinder::vec2 value );
    cinder::vec2 get_content_size( );

protected:
    float _rotation = 0.0F;
public:
    virtual void set_rotation( float value );
    float get_rotation( );

protected:
    cinder::vec2 _anchor_point = cinder::vec2( 0 );
public:
    virtual void set_anchor_point( cinder::vec2 value );
    cinder::vec2 get_anchor_point( );

protected:
    cinder::vec2 _pivot = cinder::vec2( 0 );
public:
    virtual void set_pivot( cinder::vec2 value );
    cinder::vec2 get_pivot( );

protected:
    cinder::ColorA _color = cinder::ColorA::white( );
public:
    virtual void set_color( cinder::ColorA value );
    cinder::ColorA get_color( );
    virtual void set_opacity( float alpha );
    float get_opacity( );

protected:
    std::list<std::shared_ptr<node>> _children;
public:
    std::list<std::shared_ptr<node>>& get_children( );

protected:
    std::weak_ptr<node> _parent;
public:
    virtual void set_parent( std::shared_ptr<node> const& value );
    std::shared_ptr<node> get_parent( );

protected:
    int _tag = node::INVALID_TAG;
public:
    virtual void set_tag( int value );
    int get_tag( );

private:
    int _order = 0;
public:
    virtual void set_order( int value );
    int get_order( );

protected:
    std::string _name = u8"";
public:
    virtual void set_name( std::string const& value );
    std::string get_name( );

private:
    size_t _hash = 0;

protected:
    bool _running = true;
public:
    virtual void set_running( bool value );
    bool get_running( );

protected:
    bool _visible = true;
public:
    virtual void set_visible( bool value );
    bool get_visible( );

public:
    void add_child( std::shared_ptr<node> const& value );
    std::shared_ptr<node> get_child_by_name( std::string const& name );
    std::shared_ptr<node> get_child_by_tag( int tag );
    void remove_child( std::shared_ptr<node> const& child );
    void remove_child_by_name( std::string const& name );
    void remove_child_by_tag( int tag );
    void remove_all_children( );
    void remove_from_parent( );
    void remove_from_parent_user_function( std::function<void( )> remove_user_function );
private:
    bool _own_removing = false;
    std::vector<std::function<void( )>> _remove_signal;

protected:
    bool _swallow = false;

public:
    std::shared_ptr<node> get_root( );
    std::shared_ptr<node> _get_root( );

protected:
    action::action_manager _action_manager;
public:
    void run_action( std::shared_ptr<action::action> const& action );
    std::shared_ptr<action::action> get_action_by_name( std::string const& name );
    std::shared_ptr<action::action> get_action_by_tag( int tag );
    void remove_all_actions( );
    void remove_action( std::shared_ptr<action::action> const& action );
    void remove_action_by_tag( int tag );
    void remove_action_by_name( std::string const& name );
    bool is_running_action( );

public:
    cinder::mat3 get_world_matrix( );
};
