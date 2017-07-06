#include "captured_object.h"
#include <treelike/action.hpp>
using namespace cinder;
using namespace treelike;
namespace user
{
captured_object::~captured_object( )
{

}
void captured_object::captured( softptr<node> other )
{
    _captureing = true;

    run_action( action::sequence::create( action::move_to_target::create( 0.2F, other ),
                                          action::call_func::create( [ this, other ] ( )
    {
        // 0.2F後にデリートしない場合止まって描画されてしまうためfalseにしています。
        set_visible( false );

        on_captured( other );
    } ) ) );
}
bool captured_object::is_captureing( )
{
    return _captureing;
}
}