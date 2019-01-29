#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=18) out;

in vec2 vert_textCoords[3];

out vec4 geo_position;
out vec2 geo_textCoords;

uniform mat4 u_viewProjectionMatrices[6];

void main() {
	for(int face = 0; face < 6; ++face)
	{
		gl_Layer = face;
		for(int i = 0; i < 3; i++)
		{
			geo_position = gl_in[i].gl_Position;
			geo_textCoords = vert_textCoords[i];
			gl_Position = u_viewProjectionMatrices[face] * geo_position;
			EmitVertex();
		}
		EndPrimitive();
	}
}