#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model : public IRenderable
{
private:
	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;

	const char* filename;

	void LoadNode(aiNode *node, const aiScene *scene);
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterials(const aiScene *scene);
public:
	Model(const char* filename);

	Mesh* GetMeshByIndex(size_t index);
	Texture* GetTextureByMeshIndex(size_t meshIndex);

	void Load();
	void Render();
	void Clear();

	~Model();
};

