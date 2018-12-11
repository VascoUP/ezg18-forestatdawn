#include "TerrainMesh.h"

TerrainMesh::TerrainMesh()
{
}

void TerrainMesh::Load()
{
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

	mesh = new Mesh(terrainInfo);
	mesh->Load();

	texture = new Texture("Textures/ground.jpg");
	texture->LoadTexture();

	mesh->SetTexture(texture);
}

void TerrainMesh::Render()
{
	//texture->UseTexture();
	mesh->Render();
}

void TerrainMesh::Clear()
{
	delete mesh;
	delete texture;
}


TerrainMesh::~TerrainMesh()
{
	Clear();
}
