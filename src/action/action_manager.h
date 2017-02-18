#pragma once
#include "action.h"
namespace action
{
class action_manager
{
public:
    void add_action( std::shared_ptr<action> const& action, std::shared_ptr<node> const& target, bool pause );
    std::shared_ptr<action> get_action_by_name( std::string const& name );
    std::shared_ptr<action> get_action_by_tag( int tag );
    void remove_all_actions( );
    void remove_action( std::shared_ptr<action> const& action );
    void remove_action_by_tag( int tag );
    void remove_action_by_name( std::string const& name );
    bool is_running( );
    void update( float delta );
private:
    std::list<std::shared_ptr<action>> _actions;
    std::vector<std::function<void( )>> _remove_signal;
};
}