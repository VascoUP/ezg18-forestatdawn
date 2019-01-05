#version 330

layout (location = 0) in vec3 vertPos;
uniform mat4 u_modelMatrix;

void main() {
	gl_Position = u_modelMatrix * vec4(vertPos, 1.0);
}