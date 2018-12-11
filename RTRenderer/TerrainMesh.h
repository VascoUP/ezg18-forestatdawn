#pragma once

#include "Mesh.h"
#include "Texture.h"

class TerrainMesh : 
	public IRenderable
{
private:
	Mesh* mesh;
	Texture* texture;

public:
	TerrainMesh();

	void Load();
	void Render();
	void Clear();

	~TerrainMesh();
};

