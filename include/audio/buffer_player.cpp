#include "buffer_player.h"
#include "utility/assert_log.h"
#include <cinder/app/App.h>
#include <cinder/audio/PanNode.h>
namespace audio
{
CREATE_CPP( buffer_player, std::string const& relative_path )
{
    CREATE( buffer_player, relative_path );
}
buffer_player::~buffer_player( )
{
    _buffer_player_node->disconnectAll( );
}
bool buffer_player::init( std::string const & relative_path )
{
    assert_log( !cinder::app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    auto ctx = cinder::audio::Context::master( );
    cinder::audio::SourceFileRef sourceFile = cinder::audio::load( cinder::app::loadAsset( relative_path ), ctx->getSampleRate( ) );
    cinder::audio::BufferRef buffer = sourceFile->loadBuffer( );
    _buffer_player_node = ctx->makeNode( new cinder::audio::BufferPlayerNode( buffer ) );
    _gain = ctx->makeNode( new cinder::audio::GainNode( 1.0F ) );
    _buffer_player_node >> _gain >> ctx->getOutput( );
    ctx->enable( );
    return true;
}
void buffer_player::play( )
{
    _buffer_player_node->start( );
}
void buffer_player::stop( )
{
    _buffer_player_node->stop( );
    _buffer_player_node->disconnectAll( );
}
void buffer_player::set_gain( float value )
{
    _gain->setValue( value );
}
float buffer_player::get_gain( )
{
    return _gain->getValue( );
}
}