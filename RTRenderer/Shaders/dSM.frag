#version 330

in vec2 vert_TextCoords;

uniform sampler2D u_texture;

void main() {
	if(texture(u_texture, vert_TextCoords).a <= 0.8)
		discard;
}