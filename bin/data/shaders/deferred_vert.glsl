#version 330

uniform mat4 modelViewMatrix;
uniform mat4 camModelViewMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform vec3 lightPosition;

in vec4 position;

out vec3 varyingLightPos;

void main(){
	gl_Position = modelViewProjectionMatrix * position;
	// varyingLightPos = (modelViewMatrix * vec4(lightPosition, 1.0)).xyz;
	varyingLightPos = (camModelViewMatrix * vec4(lightPosition, 0.0)).xyz;
}