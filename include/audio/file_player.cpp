#include "file_player.h"
#include "utility/assert_log.h"
#include <cinder/app/App.h>
#include <cinder/audio/PanNode.h>
namespace audio
{
CREATE_CPP( file_player, std::string const& relative_path )
{
    CREATE( file_player, relative_path );
}
file_player::~file_player( )
{
    _file_player_node->disconnectAll( );
}
bool file_player::init( std::string const & relative_path )
{
    assert_log( !cinder::app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    auto ctx = cinder::audio::Context::master( );
    cinder::audio::SourceFileRef sourceFile = cinder::audio::load( cinder::app::loadAsset( relative_path ), ctx->getSampleRate( ) );
    _file_player_node = ctx->makeNode( new cinder::audio::FilePlayerNode( sourceFile ) );
    _gain = ctx->makeNode( new cinder::audio::GainNode( 1.0F ) );
    _file_player_node >> _gain >> ctx->getOutput( );
    ctx->enable( );
    return true;
}
void file_player::play( )
{
    _file_player_node->start( );
}
void file_player::stop( )
{
    _file_player_node->stop( );
    _file_player_node->disconnectAll( );
}
void file_player::set_gain( float value )
{
    _gain->setValue( value );
}
float file_player::get_gain( )
{
    return _gain->getValue( );
}
}