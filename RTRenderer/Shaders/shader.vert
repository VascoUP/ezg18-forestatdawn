#version 330

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertMainTex;
layout (location = 2) in vec3 vertNormal;

out vec3 vert_normal;
out vec2 vert_mainTex;
out vec3 vert_pos;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main()
{
	vec4 worldPos = u_modelMatrix * vec4(vertPos, 1.0);
	gl_Position = u_projectionMatrix * u_viewMatrix * worldPos;
	
	vert_normal = mat3(u_modelMatrix) * vertNormal;
	vert_mainTex = vertMainTex;
	vert_pos = worldPos.xyz;
}