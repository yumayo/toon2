#include "action_manager.h"
#include "boost/range/algorithm/find_if.hpp"
#include "boost/range/algorithm/remove_if.hpp"
#include "../utility/assert_log.h"
#include "../node.h"
using namespace utility;
namespace action
{
void action_manager::add_action( std::shared_ptr<action> const& action, std::shared_ptr<node> const& target, bool pause )
{
    action->set_target( target );
    action->set_pause( pause );
    _actions.emplace_back( action );
}

std::shared_ptr<action> action_manager::get_action_by_name( std::string const & name )
{
    assert_log( !name.empty( ), "無効な名前です。", return std::make_shared<action>( ) );

    std::hash<std::string> h;
    size_t hash = h( name );

    auto itr = boost::find_if( _actions, [ this, hash, name ] ( std::shared_ptr<action>& act )
    {
        return act->_hash == hash && act->_name.compare( name ) == 0;
    } );

    if ( itr != boost::end( _actions ) )
    {
        return *itr;
    }
    return std::make_shared<action>( );
}

std::shared_ptr<action> action_manager::get_action_by_tag( int tag )
{
    assert_log( tag == node::INVALID_TAG, "無効なタグです。", return std::make_shared<action>( ) );

    auto itr = boost::find_if( _actions, [ this, tag ] ( std::shared_ptr<action>& act )
    {
        return act->_tag == tag;
    } );

    if ( itr != boost::end( _actions ) )
    {
        return *itr;
    }
    return std::make_shared<action>( );
}

void action_manager::remove_all_actions( )
{
    _actions.clear( );
}

void action_manager::remove_action( std::shared_ptr<action> const& act_weak )
{
    if ( _actions.empty( ) ) return;

    auto itr = boost::find_if( _actions, [ this, act_weak ] ( std::shared_ptr<action>& act )
    {
        return act == act_weak;
    } );
    _actions.erase( itr );
}

void action_manager::remove_action_by_tag( int tag )
{
    assert_log( tag == node::INVALID_TAG, "無効なタグです。", return );

    auto act = this->get_action_by_tag( tag );

    if ( act )
    {
        this->remove_action( act );
    }
    else
    {
        log( "remove_action_by_tag(tag = %d): 子供が見つかりませんでした。", tag );
    }
}

void action_manager::remove_action_by_name( std::string const & name )
{
    assert_log( !name.empty( ), "無効な名前です。", return );

    auto act = this->get_action_by_name( name );

    if ( act )
    {
        this->remove_action( act );
    }
    else
    {
        log( "remove_action_by_name(name = %s): 子供が見つかりませんでした。", name.c_str( ) );
    }
}

void action_manager::update( float delta )
{
    for ( auto const& obj : _actions )
    {
        obj->update( delta );
    }
    auto erase = boost::remove_if( _actions, [ ] ( std::shared_ptr<action>& act )
    {
        return act->is_done( );
    } );
    if ( erase != boost::end( _actions ) )
        _actions.erase( erase );
}
}