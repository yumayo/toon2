#pragma once
#include "node.h"
#include "cinder/gl/Texture.h"
namespace renderer
{
    class button : public node
    {
    public:
        CREATE_H( button, cinder::vec2 const& size );
        CREATE_H( button, std::string const& normal_texture_relative_path, std::string const& swallow_texture_relative_path = "" );
        bool init( cinder::vec2 const& size );
        bool init( std::string const& normal_texture_relative_path, std::string const& swallow_texture_relative_path = "" );
        virtual void render( ) override;
        virtual bool mouse_began( cinder::app::MouseEvent event ) override;
        virtual void mouse_moved( cinder::app::MouseEvent event ) override;
        virtual void mouse_ended( cinder::app::MouseEvent event ) override;
        virtual bool touch_began( cinder::app::TouchEvent::Touch event )override;
        virtual void touch_moved( cinder::app::TouchEvent::Touch event )override;
        virtual void touch_ended( cinder::app::TouchEvent::Touch event )override;
    public:
        std::function<void( )> on_began;
        std::function<void( )> on_ended;
    protected:
        bool _touch = false;
        cinder::gl::TextureRef _render_texture;
        cinder::gl::TextureRef _normal_texture;
        cinder::gl::TextureRef _swallow_texture;
    };
}
