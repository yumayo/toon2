#include "action_manager.h"
#include "boost/range/algorithm/find_if.hpp"
#include "boost/range/algorithm/remove_if.hpp"
#include "../utility.h"
#include "../node.h"
namespace action
{
void action_manager::add_action( action_weak const& action, node_weak const& target, bool pause )
{
    action.lock( )->set_target( target );
    action.lock( )->set_pause( pause );
    _actions.emplace_back( std::move( action.lock( ) ) );
}

action_weak action_manager::get_action_by_name( std::string const & name )
{
    assert_log( !name.empty( ), "無効な名前です。" );

    std::hash<std::string> h;
    size_t hash = h( name );

    auto itr = boost::find_if( _actions, [ this, hash, name ] ( action_ref& act )
    {
        return act->_hash == hash && act->_name.compare( name ) == 0;
    } );

    if ( itr != boost::end( _actions ) )
    {
        return *itr;
    }
    return action_weak( );
}

action_weak action_manager::get_action_by_tag( int tag )
{
    assert_log( tag != node::INVALID_TAG, "無効なタグです。" );

    auto itr = boost::find_if( _actions, [ this, tag ] ( action_ref& act )
    {
        return act->_tag == tag;
    } );

    if ( itr != boost::end( _actions ) )
    {
        return *itr;
    }
    return action_weak( );
}

void action_manager::remove_all_actions( )
{
    _actions.clear( );
}

void action_manager::remove_action( action_weak const& action )
{
    if ( _actions.empty( ) ) return;

    auto itr = boost::find_if( _actions, [ this, action ] ( action_ref& act )
    {
        return act == action.lock( );
    } );
    _actions.erase( itr );
}

void action_manager::remove_action_by_tag( int tag )
{
    assert_log( tag != node::INVALID_TAG, "無効なタグです。" );

    auto act = this->get_action_by_tag( tag );

    if ( act.lock( ) )
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
    assert_log( !name.empty( ), "無効な名前です。" );

    auto act = this->get_action_by_name( name );

    if ( act.lock( ) )
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
    auto erase = boost::remove_if( _actions, [ ] ( action_ref& act )
    {
        return act->is_done( );
    } );
    if ( erase != boost::end( _actions ) )
        _actions.erase( erase );

    for ( auto const& obj : _actions )
    {
        obj->update( delta );
    }
}
}