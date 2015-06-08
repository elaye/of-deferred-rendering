#version 330

#define PI 3.14159265358979323846

in vec3 varyingLightPos;

uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform sampler2D colorMap;

uniform vec2 res;
uniform float zNear;
uniform float zFar;
uniform float fov;
uniform float aspectRatio;

struct Light {
	vec3 color;
	float power;
};

struct Material {
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;	
};

uniform Light light;
uniform Material material;

out vec4 color;

vec3 getFarViewDir(vec2 uv) {
  float hFar = 2.0 * tan(fov / 2.0 * PI / 180.0) * zFar;
  float wFar = hFar * aspectRatio;
  vec3 dir = vec3(wFar * (uv.x - 0.5), hFar * (uv.y - 0.5), -zFar);
  return dir;
}

vec3 positionFromDepth(vec2 uv, float z) {
  vec3 ray = getFarViewDir(uv);
  return ray * z / zFar;
}

float depthEyeSpace(vec2 uv){
    float z_b = texture(depthMap, uv).x;
    float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
    return z_e;
}

void main(){
	vec2 uv = gl_FragCoord.xy / res;
	uv.y = 1.0 - uv.y;

	vec3 normal = texture(normalMap, uv).xyz;
	float depth = depthEyeSpace(uv);
	vec3 position = positionFromDepth(uv, depth);
	vec3 albedo = texture(colorMap, uv).xyz;

	vec3 N = normalize(normal);
	vec3 E = normalize(-position);
	// vec3 L = normalize(varyingLightPos - position);
	vec3 L = normalize(varyingLightPos);

	float cosTheta = clamp(dot(N, L), 0.0, 1.0);

	vec3 R = normalize(-reflect(L, N));
	float cosAlpha = clamp(dot(E, R), 0.0, 1.0);

	// Ambient
	vec3 a = albedo;
	// Diffuse
	vec3 d = material.diffuse * light.color * light.power * cosTheta;
	// Specular
	vec3 s = material.specular * light.color * light.power * pow(cosAlpha, material.shininess);
	
	vec3 linearColor = a + d + s;
	vec3 gamma = vec3(1.0/2.2);
	color = vec4(pow(linearColor, gamma), 1.0);
}