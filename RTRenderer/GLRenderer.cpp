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

bool GLRenderer::DynamicMeshes() {
	for (size_t i = 0; i < m_renderObjects.size(); i++) {
		if (m_renderObjects[i]->FilterPass(RenderFilter::R_DYNAMIC))
			return true;
	}
	return false;
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
	if (filter == RenderFilter::R_ALL) {
		printf("Render_ALL is an unsupported render mode for a shadow map pass");
		return;
	}
	
	ShadowMap* shadowMap;
	if (filter == RenderFilter::R_STATIC) {
		shadowMap = m_directionalLight->GetStaticShadowMap();
	}
	else {
		shadowMap = m_directionalLight->GetDynamicShadowMap();
	}

	// Use the directional light shadow map
	m_directionalSMShader->UseShader();

	// Set viewport to be the directional light shadow map
	glViewport(0, 0, shadowMap->GetShadowWidth(), shadowMap->GetShadowHeight());

	// Bind framebuffer to be the shadow map texture
	shadowMap->Write();

	// Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT);

	// Set uniforms
	GLuint uniformModel = m_directionalSMShader->GetModelLocation();
	m_directionalSMShader->SetDirectionalLightTransform(&m_directionalLight->CalculateLightTransform());

	// Render scene
	RenderScene(filter, uniformModel);

	// Re-bind framebuffer to the default one
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_directionalLightPassDone = true;
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
	
	m_directionalLight->GetStaticShadowMap()->Read(GL_TEXTURE1);
	m_shader->SetDirectionalStaticSM(1);

	m_directionalLight->GetDynamicShadowMap()->Read(GL_TEXTURE2);
	m_shader->SetDirectionalDynamicSM(2);

	// Render scene
	RenderScene(filter, uniformModel);
}


void GLRenderer::Render(GLWindow* glWindow, Transform* root, RenderFilter filter)
{
	if (filter != RenderFilter::R_STATIC && DynamicMeshes()) {
		// Only calculate dynamic shadow map if there are dynamic objects to display
		DirectionalSMPass(RenderFilter::R_DYNAMIC);
		glWindow->SetViewport();
	}

	RenderPass(filter);
}

void GLRenderer::BakeShadowMaps(GLWindow * glWindow)
{
	DirectionalSMPass(RenderFilter::R_STATIC);
	glWindow->SetViewport();
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

