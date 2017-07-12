#include "soft_circle.h"
#include <cinder/gl/gl.h>
#include <treelike/utility.hpp>
#include <treelike/action.hpp>
#include <cinder/Rand.h>
using namespace cinder;
using namespace treelike;
namespace user
{
CREATE_CPP( soft_circle, float radius )
{
    CREATE( soft_circle, radius );
}
bool soft_circle::init( float radius )
{
    using namespace treelike::action;

    renderer::circle::init( radius, 40.0F );

    set_schedule_update( );

    Rand rander;

    _edges = std::vector<float>( _segments, 0.0F );
    for ( auto& edge : _edges )
    {
        edge = rander.nextFloat( 0.0F, M_PI * 2 );
    }

    return true;
}
void soft_circle::render( )
{
    using namespace cinder::gl;
    using namespace std;

    const vec2 &center = vec2( _radius, _radius );
    float radius = _radius;
    int numSegments = _segments;

    auto ctx = context( );
    const GlslProg* curGlslProg = ctx->getGlslProg( );
    assert_log( curGlslProg, "No GLSL program bound", return );

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

    unique_ptr<uint8_t [ ]> data( new uint8_t[dataSizeBytes] );
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
            verts[s + 1] = center + unit * radius + unit * radius * 0.05F * ( 1.0F + std::sin( _edges[s % _segments] ) ) * 0.5F;
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
void soft_circle::update( float delta )
{
    for ( auto& edge : _edges )
    {
        edge += M_PI * 2 * delta;
    }
}
}
