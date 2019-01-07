#include "GLRenderer.h"

CubeMap* cubemap;

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
	m_omnidirectionalSMShader = new OmnidirectionalShadowMapShader();
	m_omnidirectionalSMShader->CreateFromFiles("Shaders/omniSM.vert", "Shaders/omniSM.frag", "Shaders/omniSM.geom");
	m_cubemapShader = new CubeMapRenderShader();
	m_cubemapShader->CreateFromFiles("Shaders/envReflection.vert", "Shaders/envReflection.frag", "Shaders/envReflection.geom");
	
	cubemap = new CubeMap();
	cubemap->Init(256, 256, 0.01f, 100.0f);

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
}

void GLRenderer::DirectionalSMPass(RenderFilter filter)
{
	if (filter == RenderFilter::R_ALL) {
		printf("Render_ALL is an unsupported render mode for a shadow map pass");
		return;
	}

	// Use the directional light shadow map
	m_directionalSMShader->UseShader();

	ShadowMap* shadowMap;
	if (filter == RenderFilter::R_STATIC) {
		shadowMap = m_directionalLight->GetStaticShadowMap();
	}
	else {
		shadowMap = m_directionalLight->GetDynamicShadowMap();
	}
	
	// Set viewport to be the directional light shadow map
	glViewport(0, 0, shadowMap->GetShadowWidth(), shadowMap->GetShadowHeight());

	// Bind framebuffer to be the shadow map texture
	shadowMap->Write();

	// Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT);

	// Set uniforms
	GLuint uniformModel = m_directionalSMShader->GetModelLocation();
	m_directionalSMShader->SetDirectionalLightTransform(&m_directionalLight->CalculateLightTransform());
	
	ShaderCompiler::ValidateProgram(m_directionalSMShader->GetShaderID());

	// Render scene
	RenderScene(filter, uniformModel);

	// Re-bind framebuffer to the default one
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderer::OmnidirectionalSMPass(PointLight* light, RenderFilter filter)
{
	if (filter == RenderFilter::R_ALL) {
		printf("Render_ALL is an unsupported render mode for a shadow map pass");
		return;
	}

	// Use the directional light shadow map
	m_omnidirectionalSMShader->UseShader();

	ShadowMap* shadowMap;
	if (filter == RenderFilter::R_STATIC) {
		shadowMap = light->GetStaticShadowMap();
	}
	else {
		shadowMap = light->GetDynamicShadowMap();
	}

	// Set viewport to be the directional light shadow map
	glViewport(0, 0, shadowMap->GetShadowWidth(), shadowMap->GetShadowHeight());

	// Bind framebuffer to be the shadow map texture
	shadowMap->Write();

	// Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT);

	// Set uniforms
	GLuint uniformModel = m_omnidirectionalSMShader->GetModelLocation();
	m_omnidirectionalSMShader->SetLightMatrices(light->CalculateLightTransform());
	m_omnidirectionalSMShader->SetLightPosition(&light->GetTransform()->GetPosition());
	m_omnidirectionalSMShader->SetFarPlane(light->GetFarPlane());

	ShaderCompiler::ValidateProgram(m_omnidirectionalSMShader->GetShaderID());

	// Render scene
	RenderScene(filter, uniformModel);

	// Re-bind framebuffer to the default one
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderer::CubeMapPass(Transform * transport, CubeMap * cubemap)
{
	// Use the directional light shadow map
	m_cubemapShader->UseShader();

	// Set viewport to be the directional light shadow map
	glViewport(0, 0, cubemap->GetShadowWidth(), cubemap->GetShadowHeight());

	// Bind framebuffer to be the shadow map texture
	cubemap->Write();

	// Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT);
	// Clear buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set uniforms
	m_cubemapShader->SetViewProjectMatrices(
		transport->GetCubeViewProjectionMatrices(
			cubemap->GetAspect(), 
			cubemap->GetNear(), 
			cubemap->GetFar()));
	m_cubemapShader->SetCameraPosition(&Camera::GetInstance()->GetCameraPosition());
	m_cubemapShader->SetAmbientIntensity(m_ambientIntensity);
	m_cubemapShader->SetMaterial(m_material);
	m_cubemapShader->SetDirectionalLight(m_directionalLight);
	m_cubemapShader->SetPointLights(&m_pointLights[0], m_pointLightsCount, 4, 0);
	m_cubemapShader->SetSpotLights(&m_spotLights[0], m_spotLightsCount, 4 + m_pointLightsCount, m_pointLightsCount);

	m_cubemapShader->SetTexutre(1);

	GLuint uniformModel = m_cubemapShader->GetModelLocation();

	ShaderCompiler::ValidateProgram(m_cubemapShader->GetShaderID());

	// Render scene
	RenderScene(RenderFilter::R_ALL, uniformModel);

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
	m_shader->SetProjectionMatrix(&Camera::GetInstance()->ProjectionMatrix());
	m_shader->SetViewMatrix(&Camera::GetInstance()->CalculateViewMatrix());
	m_shader->SetCameraPosition(&Camera::GetInstance()->GetCameraPosition());
	m_shader->SetAmbientIntensity(m_ambientIntensity);
	m_shader->SetMaterial(m_material);
	m_shader->SetDirectionalLight(m_directionalLight);
	m_shader->SetPointLights(&m_pointLights[0], m_pointLightsCount, 4, 0);
	m_shader->SetSpotLights(&m_spotLights[0], m_spotLightsCount, 4 + m_pointLightsCount, m_pointLightsCount);
	m_shader->SetDirectionalLightTransform(&m_directionalLight->CalculateLightTransform());
	
	m_shader->SetTexutre(1);

	m_directionalLight->GetStaticShadowMap()->Read(GL_TEXTURE2);
	m_shader->SetDirectionalStaticSM(2);

	m_directionalLight->GetDynamicShadowMap()->Read(GL_TEXTURE3);
	m_shader->SetDirectionalDynamicSM(3);

	GLuint uniformModel = m_shader->GetModelLocation();

	ShaderCompiler::ValidateProgram(m_shader->GetShaderID());

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

	CubeMapPass(root, cubemap);
	glWindow->SetViewport();

	RenderPass(filter);
}

void GLRenderer::BakeShadowMaps(GLWindow * glWindow)
{
	// Directional Light
	DirectionalSMPass(RenderFilter::R_STATIC);
	// Point Lights
	for (size_t i = 0; i < m_pointLightsCount; i++) {
		OmnidirectionalSMPass(m_pointLights[i], RenderFilter::R_STATIC);
	}
	// Spot Lights
	for (size_t i = 0; i < m_spotLightsCount; i++) {
		OmnidirectionalSMPass(m_spotLights[i], RenderFilter::R_STATIC);
	}

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