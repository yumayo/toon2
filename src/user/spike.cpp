#include "spike.h"
#include "cinder/gl/gl.h"
#include "utility.hpp"
using namespace cinder;
namespace user
{
CREATE_CPP( spike, float radius, float segments, float sharpness )
{
    CREATE( spike, radius, segments, sharpness );
}
bool spike::init( float radius, float segments, float sharpness )
{
    renderer::circle::init( radius, segments );
    set_sharpness( sharpness );
    return true;
}
void spike::render( )
{
    const vec2 &center = vec2( _radius, _radius );
    float radius = _radius;
    int numSegments = _segments;

    using namespace cinder::gl;

    auto ctx = context( );
    const GlslProg* curGlslProg = ctx->getGlslProg( );
    if ( !curGlslProg ) {
        utility::log( "No GLSL program bound" );
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

    size_t vertsOffset, texCoordsOffset, normalsOffset;
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

    std::unique_ptr<uint8_t [ ]> data( new uint8_t[dataSizeBytes] );
    vec2 *verts = ( posLoc >= 0 ) ? reinterpret_cast<vec2*>( data.get( ) + vertsOffset ) : nullptr;
    vec2 *texCoords = ( texLoc >= 0 ) ? reinterpret_cast<vec2*>( data.get( ) + texCoordsOffset ) : nullptr;
    vec3 *normals = ( normalLoc >= 0 ) ? reinterpret_cast<vec3*>( data.get( ) + normalsOffset ) : nullptr;

    if ( verts )
        verts[0] = center;
    if ( texCoords )
        texCoords[0] = vec2( 0.5f, 0.5f );
    if ( normals )
        normals[0] = vec3( 0, 0, 1 );
    const float tDelta = 1.0f / numSegments * 2 * (float)M_PI;
    float t = 0;
    for ( int s = 0; s <= numSegments; s++ ) {
        const vec2 unit( math<float>::cos( t ), math<float>::sin( t ) );
        if ( verts )
            if ( ( s & 0x1 ) == 0x0 )
                verts[s + 1] = center + unit * radius;
            else
                verts[s + 1] = center + unit * ( radius + _sharpness );
        if ( texCoords )
            texCoords[s + 1] = unit * 0.5f + vec2( 0.5f, 0.5f );
        if ( normals )
            normals[s + 1] = vec3( 0, 0, 1 );
        t += tDelta;
    }

    defaultVbo->bufferSubData( 0, dataSizeBytes, data.get( ) );
    ctx->getDefaultVao( )->replacementBindEnd( );

    ctx->setDefaultShaderVars( );
    ctx->drawArrays( GL_TRIANGLE_FAN, 0, numSegments + 2 );
    ctx->popVao( );
}
void spike::set_sharpness( float value )
{
    _sharpness = value;
}
float spike::get_sharpness( )
{
    return  _sharpness;
}
}