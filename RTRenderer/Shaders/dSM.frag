#version 330

in vec2 vert_mainTex;

uniform sampler2D u_mainTexture;

void main() {
	if(texture(u_mainTexture, vert_mainTex).a <= 0.5f)
		discard;
}