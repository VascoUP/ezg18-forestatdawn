#version 330

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertTexCoords;

uniform mat4 u_modelMatrix;
uniform mat4 u_directionalLightTransform;

out vec2 vert_TextCoords;

void main() {
	gl_Position = u_directionalLightTransform * u_modelMatrix * vec4(vertPos, 1.0);
}