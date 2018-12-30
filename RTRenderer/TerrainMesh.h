#pragma once

#include "Mesh.h"
#include "Texture.h"

class TerrainMesh : 
	public Mesh
{
private:
	TerrainMesh(MeshInfo* info);

public:
	static TerrainMesh* CreateInstance();
	~TerrainMesh();
};

