#include "feed_host.h"
#include "player.h"
#include "feed_manager_host.h"
using namespace cinder;
namespace user
{
CREATE_CPP( feed_host )
{
    CREATE( feed_host );
}
bool feed_host::init( )
{
    if ( !feed::init( ) ) return false;

    return true;
}
// ホストは自分自身で作成できます。

}