#include "user_default.h"
#include "utility/assert_log.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"
using namespace cinder;
user_default* user_default::_instans = nullptr;
Json::Value & user_default::get_root( )
{
    return _root;
}
void user_default::save( )
{
    Json::StyledWriter writer;
    auto data = writer.write( _root );
    auto dataRef = writeFile( app::getAssetPath( _target_file_name ) );
    dataRef->getStream( )->writeData( data.c_str( ), data.size( ) );
}
user_default* user_default::get_instans( )
{
    if ( !_instans ) _instans = new user_default;
    return _instans;
}
void user_default::remove_instans( )
{
    get_instans( )->save( );
    delete _instans;
    _instans = nullptr;
}
user_default::user_default( )
{
    assert_log( !app::getAssetPath( _target_file_name ).empty( ),
                "ユーザーデフォルトファイルが見つかりません。",
                return );
    Json::Reader reader;
    if ( reader.parse( app::loadString( _target_file_name ), _root ) )
    {
        // success
    }
    else
    {
        assert_log( false, "無効なJsonファイルです。", false );
    }
}
