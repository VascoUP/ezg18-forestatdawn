#version 330

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertTexCoords;

uniform mat4 u_modelMatrix;

out vec2 vert_textCoords;

void main() {
	gl_Position = u_modelMatrix * vec4(vertPos, 1.0);
	vert_textCoords = vertTexCoords;
}