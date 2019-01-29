#version 330

in vec4 geo_position;
in vec2 geo_textCoords;

uniform vec3 u_lightPos;
uniform float u_farPlane;
uniform sampler2D u_texture;

void main() {
	if(texture(u_texture, geo_textCoords).a <= 0.8)
		discard;

	float dist = length(geo_position.xyz - u_lightPos);
	gl_FragDepth = dist / u_farPlane;
}