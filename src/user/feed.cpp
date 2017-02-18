#include "feed.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "feed_manager.h"
#include "../action/action.hpp"
#include "player.h"
using namespace cinder;
namespace user
{
CREATE_CPP( feed )
{
    CREATE( feed );
}
bool feed::init( )
{
    if ( !renderer::circle::init( 12, 5 ) ) return false;

    set_name( "feed" );
    set_color( hsvToRgb( vec3( randFloat( ), randFloat( 0.5, 0.8 ), randFloat( 0.5, 0.8 ) ) ) );
    set_scale( vec2( 0.0F ) );
    float target_scale = randFloat( 1.1F, 1.3F );
    _score = ( target_scale - 1.0F ) * 10;

    using namespace action;
    auto seq = sequence::create( ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( 0.2 ) ) ),
                                 ease<EaseInOutExpo>::create( scale_by::create( 0.2F, vec2( -0.2 ) ) )
    );
    run_action( sequence::create( ease<EaseOutSine>::create( scale_to::create( 0.2F, vec2( target_scale ) ) ),
                                  repeat_forever::create( seq ) ) );

    run_action( repeat_forever::create( rotate_by::create( 5.0F, randBool( ) ? M_PI * 2 : -M_PI * 2 ) ) );

    return true;
}

void feed::render( )
{
    using namespace gl;
    using namespace std;

    // 引数達。
    const vec2 &center = vec2( _radius, _radius );
    float radius = _radius;
    int numSegments = _segments;

    auto ctx = context( );
    const GlslProg* curGlslProg = ctx->getGlslProg( );
    if ( !curGlslProg ) {
        app::console( ) << "No GLSL program bound" << endl;
        return;
    }

    ctx->pushVao( );
    ctx->getDefaultVao( )->replacementBindBegin( );

    if ( numSegments <= 0 )
        numSegments = (int)math<double>::floor( radius * M_PI * 2 );
    if ( numSegments < 3 ) numSegments = 3;
    size_t numVertices = numSegments + 2;

    size_t worstCaseSize = numVertices * sizeof( float ) * ( 2 + 2 + 3 );
    VboRef defaultVbo = ctx->getDefaultArrayVbo( worstCaseSize );
    ScopedBuffer vboScp( defaultVbo );

    size_t dataSizeBytes = 0;

    size_t vertsOffset, texCoordsOffset, normalsOffset, colorsOffset;
    int posLoc = curGlslProg->getAttribSemanticLocation( geom::Attrib::POSITION );
    if ( posLoc >= 0 ) {
        enableVertexAttribArray( posLoc );
        vertexAttribPointer( posLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)dataSizeBytes );
        vertsOffset = dataSizeBytes;
        dataSizeBytes += numVertices * 2 * sizeof( float );
    }
    int texLoc = curGlslProg->getAttribSemanticLocation( geom::Attrib::TEX_COORD_0 );
    if ( texLoc >= 0 ) {
        enableVertexAttribArray( texLoc );
        vertexAttribPointer( texLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)dataSizeBytes );
        texCoordsOffset = dataSizeBytes;
        dataSizeBytes += numVertices * 2 * sizeof( float );
    }
    int normalLoc = curGlslProg->getAttribSemanticLocation( geom::Attrib::NORMAL );
    if ( normalLoc >= 0 ) {
        enableVertexAttribArray( normalLoc );
        vertexAttribPointer( normalLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)( dataSizeBytes ) );
        normalsOffset = dataSizeBytes;
        dataSizeBytes += numVertices * 3 * sizeof( float );
    }
    int colorLoc = curGlslProg->getAttribSemanticLocation( geom::Attrib::COLOR );
    if ( colorLoc >= 0 ) {
        enableVertexAttribArray( colorLoc );
        vertexAttribPointer( colorLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)( dataSizeBytes ) );
        colorsOffset = dataSizeBytes;
        dataSizeBytes += numVertices * 4 * sizeof( float );
    }

    unique_ptr<uint8_t [ ]> data( new uint8_t[dataSizeBytes] );
    vec2 *verts = ( posLoc >= 0 ) ? reinterpret_cast<vec2*>( data.get( ) + vertsOffset ) : nullptr;
    vec2 *texCoords = ( texLoc >= 0 ) ? reinterpret_cast<vec2*>( data.get( ) + texCoordsOffset ) : nullptr;
    vec3 *normals = ( normalLoc >= 0 ) ? reinterpret_cast<vec3*>( data.get( ) + normalsOffset ) : nullptr;
    vec4 *colors = ( colorLoc >= 0 ) ? reinterpret_cast<vec4*>( data.get( ) + colorsOffset ) : nullptr;

    if ( verts )
        verts[0] = center;
    if ( texCoords )
        texCoords[0] = vec2( 0.5f, 0.5f );
    if ( normals )
        normals[0] = vec3( 0, 0, 1 );
    if ( colors )
        colors[0] = _color;
    const float tDelta = 1.0f / numSegments * 2 * (float)M_PI;
    float t = 0;
    for ( int s = 0; s <= numSegments; s++ ) {
        const vec2 unit( math<float>::cos( t ), math<float>::sin( t ) );
        if ( verts )
            verts[s + 1] = center + unit * radius;
        if ( texCoords )
            texCoords[s + 1] = unit * 0.5f + vec2( 0.5f, 0.5f );
        if ( normals )
            normals[s + 1] = vec3( 0, 0, 1 );
        if ( colors )
            colors[s + 1] = _color;
        t += tDelta;
    }

    defaultVbo->bufferSubData( 0, dataSizeBytes, data.get( ) );
    ctx->getDefaultVao( )->replacementBindEnd( );

    gl::color( ColorA( 1, 1, 1, 1 ) );
    ctx->setDefaultShaderVars( );
    ctx->drawArrays( GL_TRIANGLE_FAN, 0, numSegments + 2 );
    ctx->popVao( );
}
void feed::captured( std::weak_ptr<node> player )
{
    _captureing = true;

    auto cpt = [ this, player ]
    {
        auto pla = std::dynamic_pointer_cast<user::player>( player.lock( ) );
        pla->capture( _score );
        auto mgr = std::dynamic_pointer_cast<user::feed_manager>( get_parent( ) );
        mgr->create_feed( );
    };

    run_action( action::sequence::create( action::move_to_target::create( 0.3F, player.lock( ) ),
                                          action::call_func::create( std::move( cpt ) ),
                                          action::remove_self::create( ) ) );
}
bool feed::captureing( )
{
    return _captureing;
}
}