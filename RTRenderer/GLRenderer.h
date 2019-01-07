#pragma once

#include <vector>

#include <GL\glew.h>

#include "Commons.h"

#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "CubeMap.h"

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
	std::vector<GLObjectRenderer*> m_renderables;
	std::vector<Texture*> m_textures;

	DefaultShader* m_shader;
	DirectionalShadowMapShader* m_directionalSMShader;
	OmnidirectionalShadowMapShader* m_omnidirectionalSMShader;

	GLModelRenderer* model;
	CubeMapRenderShader* m_cubemapShader;

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

	void Initialize(Transform* transform);

	std::vector<Texture*> GetTextures();
	GLfloat GetAmbient();
	void SetAmbient(GLfloat intensity);
	DirectionalLight* GetDirectionalLight();
	void SetDirectionalLight(DirectionalLight* light);
	void AddPointLight(PointLight* light);
	void AddSpotLight(SpotLight* light);
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
	void OmnidirectionalSMPass(PointLight* light, RenderFilter filter);
	void CubeMapPass(Transform* transport, CubeMap* cubemap);
	void RenderPass(RenderFilter filter);
};
