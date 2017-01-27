#pragma once
#include "action.h"
namespace action
{
class action_manager
{
public:
    void add_action( action_weak const& action, node_weak const& target, bool pause );
    action_weak get_action_by_name( std::string const& name );
    action_weak get_action_by_tag( int tag );
    void remove_all_actions( );
    void remove_action( action_weak const& action );
    void remove_action_by_tag( int tag );
    void remove_action_by_name( std::string const& name );
    void update( float delta );
private:
    std::vector<std::shared_ptr<action>> _actions;
};
}