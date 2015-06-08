#version 330

uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 normal;

out vec4 varyingNormal;

void main(){
	gl_Position = modelViewProjectionMatrix * position;
	varyingNormal = modelViewMatrix * vec4(normal.xyz, 0.0);
	// varyingNormal = modelViewMatrix * normal;
	// varyingNormal = vec4(normal.xyz, 0.0);
}