#version 150 core

uniform vec3 uEyeDirection;

in vec4 vColor;
in vec4 vPosition;
in vec3 vNormal;

out vec4 Color;

void main( void )
{
	float d = dot(vNormal, uEyeDirection);
	Color.rgb = vColor.rgb * clamp(d, 0.25, 1.0) * 2.0;
	Color.a = 1.0;
}