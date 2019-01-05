#version 330

in vec4 geo_position;

uniform vec3 u_lightPos;
uniform float u_farPlane;

void main() {
	float dist = length(geo_position.xyz - u_lightPos);
	gl_FragDepth = dist / u_farPlane;
}