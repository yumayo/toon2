#include "captured_object.h"
#include "action.hpp"
using namespace cinder;
namespace user
{
captured_object::~captured_object( )
{

}
void captured_object::captured( std::weak_ptr<node> other )
{
    _captureing = true;

    run_action( action::sequence::create( action::move_to_target::create( 0.2F, other.lock( ) ),
                                          action::call_func::create( [ this, other ] ( ) { on_captured( other ); } ) ) );
}
bool captured_object::is_captureing( )
{
    return _captureing;
}
}