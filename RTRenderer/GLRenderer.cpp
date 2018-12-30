#include "GLRenderer.h"


GLObject::GLObject(Transform *transform, size_t modelIndex)
{
	m_transform = transform;
	m_modelIndex = modelIndex;
}

bool GLObject::FilterPass(RenderFilter filter)
{
	return (filter == R_ALL ||
		(filter == R_DYNAMIC && !m_transform->GetStatic()) ||
		(filter == R_STATIC && m_transform->GetStatic()));
}

size_t GLObject::GetModelIndex() const
{
	return m_modelIndex;
}

glm::mat4 GLObject::GetTransformMatrix() const
{
	return m_transform->TransformMatrix(true);
}



void GLObjectRenderer::AddMeshRenderer(GLObject * meshRenderer)
{
	m_objects.push_back(meshRenderer);
}

void GLObjectRenderer::Clear()
{
	for (size_t i = 0; i < m_objects.size(); i++) {
		free(m_objects[i]);
	}
	m_objects.clear();
	if(m_renderable)
		free(m_renderable);
	m_renderable = nullptr;
}

GLObjectRenderer::~GLObjectRenderer()
{
	Clear();
}



bool RenderMesh(Mesh* mesh, GLObject* meshRenderer, RenderFilter filter, GLuint uniformModel) {
	if (!meshRenderer->FilterPass(filter))
		return false;

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(meshRenderer->GetTransformMatrix()));
	mesh->Render();
	return true;
}



void GLModelRenderer::SetRenderable(Model * renderable)
{
	m_renderable = renderable;
}

void GLModelRenderer::Render(RenderFilter filter, GLuint uniformModel)
{
	Model* model = (Model*)m_renderable;

	size_t i = 0;
	Mesh* mesh = model->GetMeshByIndex(i);
	if (mesh == nullptr)
		return;

	Texture* tex = model->GetTextureByMeshIndex(i);
	if (tex != nullptr)
		tex->UseTexture();

	std::vector<GLObject*> renderableMeshes;
	for (size_t j = 0; j < m_objects.size(); j++) {
		if (RenderMesh(mesh, m_objects[j], filter, uniformModel))
			renderableMeshes.push_back(m_objects[j]);
	}

	if (renderableMeshes.size() <= 0)
		return;

	while(mesh != nullptr) {
		Mesh* mesh = model->GetMeshByIndex(++i);
		if (mesh == nullptr)
			return;

		Texture* tex = model->GetTextureByMeshIndex(i);
		if (tex != nullptr)
			tex->UseTexture();

		for (size_t j = 0; j < renderableMeshes.size(); j++) {
			RenderMesh(mesh, renderableMeshes[j], filter, uniformModel);
		}
	}
}



void GLMeshRenderer::SetRenderable(Mesh * renderable)
{
	m_renderable = renderable;
}

void GLMeshRenderer::Render(RenderFilter filter, GLuint uniformModel)
{
	Mesh* mesh = (Mesh*)m_renderable;
	Texture* tex = mesh->GetTexture();
	if (tex != nullptr)
		tex->UseTexture();

	for (size_t i = 0; i < m_objects.size(); i++) {
		RenderMesh(mesh, m_objects[i], filter, uniformModel);
	}
}



GLRenderer::GLRenderer()
{
	m_directionalSMShader = new DirectionalShadowMapShader();
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
	//m_models.push_back(mesh);
	//mesh->SetIndex(m_models.size() - 1);
}

void GLRenderer::AddObjectRenderer(GLObjectRenderer * renderer)
{
	m_renderables.push_back(renderer);
	renderer->SetIndex(m_renderables.size() - 1);
}

void GLRenderer::AddMeshRenderer(GLObject * meshRenderer)
{
	size_t index = meshRenderer->GetModelIndex();
	if (index < m_renderables.size())
		m_renderables[index]->AddMeshRenderer(meshRenderer);
}

void GLRenderer::AddTexture(const char* texLocation)
{
	Texture* tex = new Texture(texLocation);
	tex->LoadTexture();
	m_textures.push_back(tex);
}

void GLRenderer::AddShader(DefaultShader * shader)
{
	this->m_shader = shader;
}

bool GLRenderer::DynamicMeshes() {
	// Todo: Fix function
	//for (size_t i = 0; i < m_renderObjects.size(); i++) {
	//	if (m_renderObjects[i]->FilterPass(RenderFilter::R_DYNAMIC))
	return true;
	//}
	//return false;
}

void GLRenderer::RenderScene(RenderFilter filter, GLuint uniformModel) {
	for (size_t i = 0; i < m_renderables.size(); i++)
		m_renderables[i]->Render(filter, uniformModel);
	/*
	for (size_t i = 0; i < m_renderObjects.size(); i++) {
		int modelIndex = m_renderObjects[i]->GetModelIndex();

		if (modelIndex < 0 || modelIndex >= m_models.size() ||
			!m_renderObjects[i]->FilterPass(filter))
			continue;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_renderObjects[i]->GetTransformMatrix()));
		m_models[modelIndex]->Render();
	}
	*/
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
	m_shader->SetProjectionMatrix(&Camera::GetInstance()->ProjectionMatrix());
	m_shader->SetViewMatrix(&Camera::GetInstance()->CalculateViewMatrix());
	m_shader->SetCameraPosition(&Camera::GetInstance()->GetCameraPosition());
	m_shader->SetAmbientIntensity(m_ambientIntensity);
	m_shader->SetMaterial(m_material);
	m_shader->SetDirectionalLight(m_directionalLight);
	m_shader->SetPointLights(&m_pointLights[0], m_pointLightsCount);
	m_shader->SetSpotLights(&m_spotLights[0], m_spotLightsCount);
	m_shader->SetDirectionalLightTransform(&m_directionalLight->CalculateLightTransform());
	
	m_directionalLight->GetStaticShadowMap()->Read(GL_TEXTURE1);
	m_shader->SetDirectionalStaticSM(1);

	m_directionalLight->GetDynamicShadowMap()->Read(GL_TEXTURE2);
	m_shader->SetDirectionalDynamicSM(2);

	GLuint uniformModel = m_shader->GetModelLocation();

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

	/*for (size_t i = 0; i < m_renderObjects.size(); i++) {
		if (!m_renderObjects[i])
			continue;
		delete m_renderObjects[i];
	}

	for (size_t i = 0; i < m_models.size(); i++) {
		if (!m_models[i])
			continue;
		delete m_models[i];
	}*/

	for (size_t i = 0; i < m_renderables.size(); i++) {
		if (!m_renderables[i])
			continue;
		delete m_renderables[i];
	}

	for (size_t i = 0; i < m_textures.size(); i++) {
		if (!m_textures[i])
			continue;
		delete m_textures[i];
	}
}

