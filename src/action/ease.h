#pragma once
#include "action.h"
#include "finite_time_action.h"
#include "cinder/Easing.h"
namespace action
{
template<typename ease_type>
class ease : public action
{
public:
    static std::shared_ptr<ease<ease_type>> create( std::shared_ptr<finite_time_action> const& time_action );
    bool init( std::shared_ptr<finite_time_action> const& time_action );
public:
    virtual void setup( ) override;
    virtual bool is_done( ) override;
    virtual void update( float delta ) override;
protected:
    ease_type ease_object;
    std::shared_ptr<finite_time_action> _time_action;
};

template<typename ease_type>
inline std::shared_ptr<ease<ease_type>> ease<ease_type>::create( std::shared_ptr<finite_time_action> const & time_action )
{
    auto ret = std::make_shared<ease<ease_type>>( );
    if ( ret && ret->init( time_action ) );
    else ret.reset( );
    return std::move( ret );
}
template<typename ease_type>
inline bool ease<ease_type>::init( std::shared_ptr<finite_time_action> const & time_action )
{
    _time_action = time_action;
    return true;
}
template<typename ease_type>
inline void ease<ease_type>::setup( )
{
    _time_action->setup( _target, _pause );
}
template<typename ease_type>
inline bool ease<ease_type>::is_done( )
{
    return _time_action->is_done( );
}
template<typename ease_type>
inline void ease<ease_type>::update( float delta )
{
    _time_action->set_time( _time_action->get_time( ) + delta );
    _time_action->step( ease_object( _time_action->get_time( ) / _time_action->get_duration( ) ) );
}
}