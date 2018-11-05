#version 330

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertMainTex;
layout (location = 2) in vec3 vertNormal;

out vec3 fragNormal;
out vec2 fragMainTex;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 worldPos = model * vec4(vertPos, 1.0);
	gl_Position = projection * view * worldPos;
	
	// Use model to account for any changes in rotation and scaling
	// Translate is not needed to take into account because the normal is a direction
	fragNormal = mat3(model) * vertNormal;

	fragMainTex = vertMainTex;
	fragPos = worldPos.xyz;
}