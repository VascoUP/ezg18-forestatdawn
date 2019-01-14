#version 330

layout(location = 0) in vec3 pos;

out vec3 vert_texCoords;

uniform mat4 u_viewProjectionMatrix;

void main() {
	vert_texCoords = pos;
	gl_Position = u_viewProjectionMatrix * vec4(pos, 1.0);
}