#version 150 core

uniform mat4 ciModelViewProjection;
uniform mat4 ciModelMatrix;
uniform mat3 ciNormalMatrix;
uniform mat4 ciModelView;

in vec4	ciColor;
in vec4	ciPosition;
in vec3	ciNormal;

out vec4 vColor;
out vec4 vPosition;
out vec3 vNormal;

void main( void )
{
	vColor			= ciColor;
	vPosition		= ciModelView * ciPosition;
	vNormal			= normalize( ciNormalMatrix * ciNormal );
	gl_Position		= ciModelViewProjection * ciPosition;
}