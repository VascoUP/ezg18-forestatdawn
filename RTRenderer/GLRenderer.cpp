#include "GLRenderer.h"

GLRenderer::GLRenderer()
{
	m_material = new Material(0.8f, 256, 1.0f, 1.0f, 1.0f);
	m_ambientIntensity = 0.1f;
	m_pointLightsCount = 0;
}

std::vector<Texture*> GLRenderer::GetTextures()
{
	return m_textures;
}

void GLRenderer::SetAmbient(GLfloat intensity)
{
	m_ambientIntensity = intensity;
}

void GLRenderer::SetDirectionalLight(DirectionalLight * light)
{
	m_directionalLight = light;
}

void GLRenderer::AddPointLight(PointLight* light) {
	m_pointLights[m_pointLightsCount] = light;
	if (m_pointLightsCount <= MAX_POINT_LIGHTS)
		m_pointLightsCount++;
}

void GLRenderer::AddSpotLight(SpotLight * light)
{
	m_spotLights[m_spotLightsCount] = light;
	if (m_spotLightsCount <= MAX_POINT_LIGHTS)
		m_spotLightsCount++;
}

void GLRenderer::AddModels(IRenderable * mesh)
{
	m_models.push_back(mesh);
	mesh->SetIndex(m_models.size() - 1);
}

void GLRenderer::AddMeshRenderer(MeshRenderer * meshRenderer)
{
	m_renderObjects.push_back(meshRenderer);
}

void GLRenderer::AddTexture(const char* texLocation)
{
	Texture* tex = new Texture(texLocation);
	tex->LoadTexture();
	m_textures.push_back(tex);
}

void GLRenderer::AddShader(Shader * shader)
{
	//shaders.push_back(shader);
	this->m_shader = shader;
}

void GLRenderer::Render(RenderFilter filter)
{
	GLuint uniformModel = 0, uniformView = 0, uniformProjection = 0, uniformAmbientColor = 0, uniformAmbientIntensity = 0, uniformCameraPosition = 0, uniformAmbienteIntensity = 0;

	m_shader->UseShader();
	uniformModel = m_shader->GetModelLocation();
	uniformView = m_shader->GetViewLocation();
	uniformProjection = m_shader->GetProjectionLocation();
	uniformCameraPosition = m_shader->GetCameraPositionLocation();
	uniformAmbienteIntensity = m_shader->GetAmbienteIntensityLocation();

	for (size_t i = 0; i < m_renderObjects.size(); i++) {
		int modelIndex = m_renderObjects[i]->GetModelIndex();

		if (modelIndex < 0 || modelIndex >= m_models.size() || 
			!m_renderObjects[i]->FilterPass(filter))
			continue;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_renderObjects[i]->GetTransformMatrix()));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->CalculateViewMatrix()));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->ProjectionMatrix()));

		glUniform3f(uniformCameraPosition, Camera::GetInstance()->GetCameraPosition().x, Camera::GetInstance()->GetCameraPosition().y, Camera::GetInstance()->GetCameraPosition().z);
		glUniform1f(uniformAmbienteIntensity, m_ambientIntensity);

		m_shader->SetDirectionalLight(m_directionalLight);
		m_shader->SetPointLights(&m_pointLights[0], m_pointLightsCount);
		m_shader->SetSpotLights(&m_spotLights[0], m_spotLightsCount);
		m_shader->SetMaterial(m_material);

		m_models[modelIndex]->Render();
	}
}


GLRenderer::~GLRenderer()
{
	delete m_shader;
	delete m_directionalLight;

	for (size_t i = 0; i < m_pointLightsCount; i++) {
		delete m_pointLights[i];
	}

	for (size_t i = 0; i < m_spotLightsCount; i++) {
		delete m_spotLights[i];
	}

	for (size_t i = 0; i < m_renderObjects.size(); i++) {
		if (!m_renderObjects[i])
			continue;
		delete m_renderObjects[i];
	}

	for (size_t i = 0; i < m_models.size(); i++) {
		if (!m_models[i])
			continue;
		delete m_models[i];
	}

	for (size_t i = 0; i < m_textures.size(); i++) {
		if (!m_textures[i])
			continue;
		delete m_textures[i];
	}
}

