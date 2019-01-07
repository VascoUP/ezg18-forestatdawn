#version 330

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertMainTex;
layout (location = 2) in vec3 vertNormal;

uniform mat4 u_modelMatrix;

out vec3 vert_normal;
out vec2 vert_texCoord;

void main() {
	gl_Position = u_modelMatrix * vec4(vertPos, 1.0);
		
	vert_normal = mat3(u_modelMatrix) * vertNormal;
	vert_texCoord = vertMainTex;
}