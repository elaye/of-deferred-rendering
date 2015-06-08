#version 330

uniform vec4 globalColor;

in vec4 varyingNormal;

layout(location = 0) out vec4 normal;
layout(location = 1) out vec4 depth;
layout(location = 2) out vec4 albedo;

void main(){
	normal = vec4(varyingNormal.xyz, 1.0);
	depth = vec4(vec3(gl_FragCoord.z), 1.0);
	albedo = globalColor;
}