#version 150 core

uniform vec3 uEyeDirection;

in vec4 vColor;
in vec4 vPosition;
in vec3 vNormal;

out vec4 Color;

void main( void )
{
	float d = dot(vNormal, uEyeDirection);
	Color = vec4(vColor.rgb * clamp(d, 0.5, 1.0), 1.0);
}