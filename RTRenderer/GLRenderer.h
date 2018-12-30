#pragma once

#include <vector>

#include <GL\glew.h>

#include "Commons.h"

#include "Mesh.h"
#include "Model.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"

class GLObject
{
private:
	Transform* m_transform;
	size_t m_modelIndex;
public:
	GLObject(Transform *transform, size_t modelIndex);

	bool FilterPass(RenderFilter filter);
	size_t GetModelIndex() const;
	glm::mat4 GetTransformMatrix() const;

	~GLObject();
};

class GLObjectRenderer {
protected:
	bool m_useInstanciation = false;
	
	IRenderable* m_renderable;
	std::vector<GLObject*> m_objects;
public:
	virtual void Render(RenderFilter filter, GLuint uniformModel) = 0;

	void AddMeshRenderer(GLObject* meshRenderer);
	void SetIndex(size_t index) { m_renderable->SetIndex(index); }
	void Clear();

	~GLObjectRenderer();
};

class GLModelRenderer 
	: public GLObjectRenderer
{
public:
	void SetRenderable(Model* renderable);
	void Render(RenderFilter filter, GLuint uniformModel);
};

class GLMeshRenderer
	: public GLObjectRenderer
{
public:
	void SetRenderable(Mesh* renderable);
	void Render(RenderFilter filter, GLuint uniformModel);
};

class GLRenderer
{
private:
	// Todo: Erase this one
	//std::vector<IRenderable*> m_models;
	std::vector<GLObjectRenderer*> m_renderables;
	
	//std::vector<std::vector<Mesh*>> m_objects;
	//std::vector<MeshRenderer*> m_renderObjects;
	std::vector<Texture*> m_textures;

	DefaultShader* m_shader;
	DirectionalShadowMapShader* m_directionalSMShader;
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
	void AddObjectRenderer(GLObjectRenderer* renderer);
	void AddMeshRenderer(GLObject * meshRenderer);
	void AddTexture(const char* texLocation);
	void AddShader(DefaultShader* shader);
	void Render(GLWindow* glWindow, Transform* root, RenderFilter filter);
	void BakeShadowMaps(GLWindow* glWindow);

	~GLRenderer();

private:
	bool DynamicMeshes();
	void RenderScene(RenderFilter filter, GLuint uniformModel);
	void DirectionalSMPass(RenderFilter filter);
	void RenderPass(RenderFilter filter);
};
