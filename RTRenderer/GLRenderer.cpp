#include "GLRenderer.h"

GLRenderer::GLRenderer()
{
	m_directionalSMShader = new Shader();
	m_directionalSMShader->CreateFromFiles("Shaders/dSM.vert", "Shaders/dSM.frag");
	
	m_material = new Material(0.8f, 256, 1.0f, 1.0f, 1.0f);
	m_ambientIntensity = 0.1f;
	m_pointLightsCount = 0;
}

std::vector<Texture*> GLRenderer::GetTextures()
{
	return m_textures;
}

GLfloat GLRenderer::GetAmbient()
{
	return m_ambientIntensity;
}

void GLRenderer::SetAmbient(GLfloat intensity)
{
	m_ambientIntensity = intensity;
}

DirectionalLight * GLRenderer::GetDirectionalLight()
{
	return m_directionalLight;
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
	this->m_shader = shader;
}

void GLRenderer::RenderScene(RenderFilter filter, GLuint uniformModel) {
	for (size_t i = 0; i < m_renderObjects.size(); i++) {
		int modelIndex = m_renderObjects[i]->GetModelIndex();

		if (modelIndex < 0 || modelIndex >= m_models.size() ||
			!m_renderObjects[i]->FilterPass(filter))
			continue;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_renderObjects[i]->GetTransformMatrix()));
		m_models[modelIndex]->Render();
	}
}

void GLRenderer::DirectionalSMPass(RenderFilter filter)
{
	// Use the directional light shadow map
	m_directionalSMShader->UseShader();

	// Set viewport to be the directional light shadow map
	glViewport(0, 0, 
		m_directionalLight->GetShadowMap()->GetShadowWidth(), 
		m_directionalLight->GetShadowMap()->GetShadowHeight());

	// Bind framebuffer to be the shadow map texture
	m_directionalLight->GetShadowMap()->Write();

	// Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT);

	// Set uniforms
	GLuint uniformModel = m_directionalSMShader->GetModelLocation();
	m_directionalSMShader->SetDirectionalLightTransform(&m_directionalLight->CalculateLightTransform());

	// Render scene
	RenderScene(filter, uniformModel);

	// Re-bind framebuffer to the default one
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderer::RenderPass(RenderFilter filter)
{
	// Use the developed default shader
	m_shader->UseShader();

	// Clear buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set uniforms
	GLuint uniformModel = 0, uniformView = 0, uniformProjection = 0, uniformAmbientColor = 0, 
		uniformAmbientIntensity = 0, uniformCameraPosition = 0, uniformAmbienteIntensity = 0;

	uniformModel = m_shader->GetModelLocation();
	uniformView = m_shader->GetViewLocation();
	uniformProjection = m_shader->GetProjectionLocation();
	uniformCameraPosition = m_shader->GetCameraPositionLocation();
	uniformAmbienteIntensity = m_shader->GetAmbienteIntensityLocation();

	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->CalculateViewMatrix()));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->ProjectionMatrix()));
	glUniform3f(uniformCameraPosition, Camera::GetInstance()->GetCameraPosition().x, Camera::GetInstance()->GetCameraPosition().y, Camera::GetInstance()->GetCameraPosition().z);
	glUniform1f(uniformAmbienteIntensity, m_ambientIntensity);

	m_shader->SetMaterial(m_material);
	m_shader->SetDirectionalLight(m_directionalLight);
	m_shader->SetPointLights(&m_pointLights[0], m_pointLightsCount);
	m_shader->SetSpotLights(&m_spotLights[0], m_spotLightsCount);
	m_shader->SetDirectionalLightTransform(&m_directionalLight->CalculateLightTransform());
	
	m_directionalLight->GetShadowMap()->Read(GL_TEXTURE1);
	m_shader->SetDirectionalSM(1);

	// Render scene
	RenderScene(filter, uniformModel);
}


void GLRenderer::Render(GLWindow* glWindow, RenderFilter filter)
{
	if (!m_directionalLightPassDone) {
		DirectionalSMPass(filter);
		glWindow->SetViewport();
		m_directionalLightPassDone = true;
	}
	RenderPass(filter);
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

