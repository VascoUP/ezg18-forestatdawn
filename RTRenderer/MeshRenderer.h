#pragma once

#include <vector>

#include <GL\glew.h>

#include "Renderable.h"
#include "Texture.h"
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include "Material.h"

class MeshRenderer
{
private:
	std::vector<Renderable*> models;
	std::vector<Texture*> textures;
	//std::vector<Shader*> shaders;
	Shader* shader;
	Material* mat;

	Light* directionalLight;
public:
	MeshRenderer();

	std::vector<Texture*> GetTextures();
	void SetLight(Light* light);
	void AddModels(Renderable* mesh);
	void AddTexture(const char* texLocation);
	void AddShader(Shader* shader);
	void Render();

	~MeshRenderer();
};

