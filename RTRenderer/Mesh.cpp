#include "Mesh.h"

Mesh::Mesh(MeshInfo * info)
	: IRenderable()
{
	meshInfo = info;

	VAO = 0;
	VBO = 0;
	EBO = 0;
	indexCount = 0;
	texture = nullptr;
}

void Mesh::SetTexture(Texture * tex)
{
	texture = tex;
}

void Mesh::Load() {
	indexCount = meshInfo->numOfIndices;

	// Bind mesh values
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(meshInfo->indices[0]) * meshInfo->numOfIndices, meshInfo->indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(meshInfo->vertices[0]) * meshInfo->numOfVertices, meshInfo->vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(meshInfo->vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(meshInfo->vertices[0]) * 8, (void*)(sizeof(meshInfo->vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(meshInfo->vertices[0]) * 8, (void*)(sizeof(meshInfo->vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	// Unbind this mesh's values
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	delete meshInfo;
}

void Mesh::Render() {
	// Make sure that there is a mesh
	if (indexCount == 0)
		return;

	if (texture)
		texture->UseTexture();

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Clear() {
	if (EBO != 0) {
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh()
{
	Clear();
}
