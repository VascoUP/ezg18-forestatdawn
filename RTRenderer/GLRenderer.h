#pragma once

#include <vector>

#include <GL\glew.h>

#include "Commons.h"

#include "IRenderable.h"
#include "MeshRenderer.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"

class GLRenderer
{
private:
	std::vector<IRenderable*> m_models;
	std::vector<MeshRenderer*> m_renderObjects;
	std::vector<Texture*> m_textures;
	//std::vector<Shader*> shaders;

	Shader* m_shader;
	Shader* m_directionalSMShader;
	bool m_directionalLightPassDone = false;
	Material* m_material;

	GLfloat m_ambientIntensity;

	DirectionalLight* m_directionalLight;
	int m_pointLightsCount = 0;
	PointLight* m_pointLights[MAX_POINT_LIGHTS];
	int m_spotLightsCount = 0;
	SpotLight* m_spotLights[MAX_SPOT_LIGHTS];
public:
	GLRenderer();

	std::vector<Texture*> GetTextures();
	GLfloat GetAmbient();
	void SetAmbient(GLfloat intensity);
	DirectionalLight* GetDirectionalLight();
	void SetDirectionalLight(DirectionalLight* light);
	void AddPointLight(PointLight* light);
	void AddSpotLight(SpotLight* light);
	void AddModels(IRenderable* mesh);
	void AddMeshRenderer(MeshRenderer* meshRenderer);
	void AddTexture(const char* texLocation);
	void AddShader(Shader* shader);
	void Render(GLWindow* glWindow, RenderFilter filter);

	~GLRenderer();

private:
	void RenderScene(RenderFilter filter, GLuint uniformModel);
	void DirectionalSMPass(RenderFilter filter);
	void RenderPass(RenderFilter filter);
};

