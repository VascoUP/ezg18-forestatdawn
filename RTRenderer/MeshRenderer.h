#pragma once

#include <vector>

#include <GL\glew.h>

#include "Commons.h"

#include "Renderable.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Texture.h"
#include "Shader.h"
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

	GLfloat ambientIntensity;

	DirectionalLight* directionalLight;
	PointLight* pointLights[MAX_POINT_LIGHTS];
	int pointLightsCount = 0;
public:
	MeshRenderer();

	std::vector<Texture*> GetTextures();
	void SetDirectionalLight(DirectionalLight* light);
	void AddPointLight(PointLight* light);
	void AddModels(Renderable* mesh);
	void AddTexture(const char* texLocation);
	void AddShader(Shader* shader);
	void Render();

	~MeshRenderer();
};

