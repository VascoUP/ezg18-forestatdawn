#include "TerrainMesh.h"

TerrainMesh::TerrainMesh(MeshInfo* info)
	: Mesh(info) {}

TerrainMesh * TerrainMesh::CreateInstance()
{
	Texture* texture = new Texture("Textures/ground.jpg");
	texture->LoadTexture();

	GLfloat vertices[]{
		-100.0f, 0.0f, -100.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		100.0f, 0.0f, -100.0f,		50.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		100.0f, 0.0f, 100.0f, 		50.0f, 50.0f,	0.0f, -1.0f, 0.0f,
		-100.0f, 0.0f, 100.0f,		0.0f, 50.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int indices[]{
		0, 1, 2,
		0, 2, 3
	};

	MeshInfo* terrainInfo = new MeshInfo();
	terrainInfo->vertices = vertices; terrainInfo->numOfVertices = 32;
	terrainInfo->indices = indices; terrainInfo->numOfIndices = 6;

	TerrainMesh* mesh = new TerrainMesh(terrainInfo);
	mesh->Load();
	mesh->SetTexture(texture);

	return mesh;
}

TerrainMesh::~TerrainMesh()
{
	Clear();
}
