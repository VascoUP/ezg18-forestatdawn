#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=18) out;

uniform mat4 u_viewProjectionMatrices[6];

in vec3 vert_normal[];
in vec2 vert_texCoord[];

out vec3 geo_position;
out vec3 geo_normal;
out vec2 geo_texCoord;

void main() {
	for(int face = 0; face < 6; ++face)
	{
		gl_Layer = face;
		for(int i = 0; i < 3; i++)
		{
			geo_position = gl_in[i].gl_Position.xyz;
			geo_normal = vert_normal[i];
			geo_texCoord = vert_texCoord[i]; 
			gl_Position = u_viewProjectionMatrices[face] * gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
}