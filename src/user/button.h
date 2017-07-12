#pragma once
#include <treelike/node.h>
#include "cinder/gl/Texture.h"
#pragma warning(disable : 4250) // 安全にダイヤモンド継承をしているので。
namespace user
{
class base_button : public treelike::node
{
public:
    virtual ~base_button( ) {}
    virtual bool hit_touch( cinder::vec2 touch_position ) = 0;
    virtual void in_action( ) = 0;
    virtual void out_action( ) = 0;
    bool init( );
    bool began( cinder::vec2 touch_position );
    void moved( cinder::vec2 touch_position );
    void ended( cinder::vec2 touch_position );
public:
    void update( float delta ) final override;
    bool mouse_began( cinder::app::MouseEvent event ) final override;
    void mouse_moved( cinder::app::MouseEvent event ) final override;
    void mouse_ended( cinder::app::MouseEvent event ) final override;
    bool touch_began( cinder::app::TouchEvent::Touch event ) final override;
    void touch_moved( cinder::app::TouchEvent::Touch event ) final override;
    void touch_ended( cinder::app::TouchEvent::Touch event ) final override;
public:
    std::function<void( )> on_began;
    std::function<void( )> on_ended;
private:
    bool _touch = false;
    bool _prev_touch = false;
};
class base_rect_button : virtual public base_button
{
public:
    virtual ~base_rect_button( ) {}
    bool hit_touch( cinder::vec2 touch_position ) final override;
};
class base_ciecle_button : virtual public base_button
{
public:
    virtual ~base_ciecle_button( ) {}
    bool hit_touch( cinder::vec2 touch_position ) final override;
};
class base_anytype_button_opacity_action : virtual public base_button
{
public:
    virtual ~base_anytype_button_opacity_action( ) {}
    bool init( );
    virtual void in_action( ) final override;
    virtual void out_action( ) final override;
};
class base_anytype_button_scale_action : virtual public base_button
{
public:
    virtual ~base_anytype_button_scale_action( ) {}
    bool init( );
    virtual void in_action( ) final  override;
    virtual void out_action( ) final override;
};
class rect_button_opacity_action : public base_rect_button, public base_anytype_button_opacity_action
{
public:
    CREATE_H( rect_button_opacity_action );
};
class circle_button_opacity_action : public base_ciecle_button, public base_anytype_button_opacity_action
{
public:
    CREATE_H( circle_button_opacity_action );
};
class rect_button_scale_action : public base_rect_button, public base_anytype_button_scale_action
{
public:
    CREATE_H( rect_button_scale_action );
};
class circle_button_scale_action : public base_ciecle_button, public base_anytype_button_scale_action
{
public:
    CREATE_H( circle_button_scale_action );
};
}
