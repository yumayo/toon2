#include "captured_object.h"
#include "../action/action.hpp"
using namespace cinder;
namespace user
{
void captured_object::captured( std::weak_ptr<node> other )
{
    _captureing = true;

    run_action( action::sequence::create( action::move_to_target::create( 0.2F, other.lock( ) ),
                                          action::call_func::create( 
                                              [ this, other ] ( ) { on_captured( other ); } ),
                                          action::remove_self::create( ) ) );
}
bool captured_object::is_captureing( )
{
    return _captureing;
}
}