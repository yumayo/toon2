#include "sequence.h"
namespace action
{
bool sequence::init( )
{
    _target_action = _actions.begin( );
    return true;
}
void sequence::setup( )
{
    if ( is_done( ) ) return;
    ( *_target_action )->setup( _target.lock( ), _pause );
}
bool sequence::is_done( )
{
    return _target_action == _actions.end( );
}
void sequence::update( float delta )
{
    if ( is_done( ) ) return;

    ( *_target_action )->update( delta );

    if ( ( *_target_action )->is_done( ) )
    {
        _target_action++;

        setup( );
    }
}
void sequence::restart( )
{
    timeline::restart( );
    if ( is_done( ) )
    {
        for ( auto const& act : _actions )
        {
            act->restart( );
        }
        init( );
        setup( );
    }
}
}