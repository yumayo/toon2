#version 150 core

uniform vec3 uLightPos;
uniform sampler2DShadow uShadowMap;

uniform vec3 uLightDirection;

in vec4 vColor;
in vec4 vPosition;
in vec3 vNormal;

in vec4 vShadowCoord;

out vec4 Color;

void main( void )
{
	vec3 Normal			= normalize( vNormal );
	
	float NdotL			= max( dot( vNormal, LightVec ), 0.0 );
	
	vec3 Diffuse		= vec3( NdotL );
	vec3 Ambient		= vec3( 0.3 );
	
	float Shadow		= 1.0;
	
	vec4 ShadowCoord	= vShadowCoord / vShadowCoord.w;
	if ( ShadowCoord.z > -1 && ShadowCoord.z < 1 ) 
	{
		Shadow = textureProj( uShadowMap, ShadowCoord, -0.00005 );
	}

	float d = dot(vNormal, uLightDirection);
	vec4 col = vec4(vColor.rgb * clamp(d, 0.5, 1.0), 1.0);
	Color.rgb = ( Diffuse * Shadow + Ambient ) * col.rgb;
	Color.a	= 1.0;
}