#version 330

in vec3 vert_texCoords;

out vec4 frag_color;

uniform samplerCube u_skybox;

void main() {
	frag_color = texture(u_skybox, vert_texCoords);
}