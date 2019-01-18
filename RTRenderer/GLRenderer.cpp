#include "GLRenderer.h"

bool RenderMesh(Mesh* mesh, GLObject* meshRenderer, RenderFilter filter, GLuint uniformModel, LightedShader* shader = nullptr) {
	if (!meshRenderer->FilterPass(filter))
		return false;

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(meshRenderer->GetTransformMatrix()));
	
	if (shader != nullptr)
		meshRenderer->UseMaterial(shader);

	mesh->Render();
	return true;
}


GLObject::GLObject(Transform *transform, Material* material, size_t modelIndex)
{
	m_transform = transform;
	m_material = material;
	m_modelIndex = modelIndex;
}

bool GLObject::FilterPass(RenderFilter filter)
{
	return (filter == R_ALL ||
		(filter == R_DYNAMIC && !m_transform->GetStatic()) ||
		(filter == R_STATIC && m_transform->GetStatic()));
}

void GLObject::UseMaterial(LightedShader * shader)
{
	shader->SetMaterial(m_material);
}

size_t GLObject::GetModelIndex() const
{
	return m_modelIndex;
}

glm::mat4 GLObject::GetTransformMatrix() const
{
	return m_transform->TransformMatrix(true);
}

GLObject::~GLObject()
{
	//delete m_material;
}


void GLObjectRenderer::AddMeshRenderer(GLObject * meshRenderer)
{
	m_objects.push_back(meshRenderer);
}

void GLObjectRenderer::Clear()
{
	for (size_t i = 0; i < m_objects.size(); i++)
		delete m_objects[i];
	m_objects.clear();
	if(m_renderable)
		delete m_renderable;
	m_renderable = nullptr;
}

GLObjectRenderer::~GLObjectRenderer()
{
	Clear();
}


void GLModelRenderer::SetRenderable(Model * renderable)
{
	m_renderable = renderable;
}

void GLModelRenderer::Render(RenderFilter filter, GLuint uniformModel, LightedShader* shader)
{
	Model* model = (Model*)m_renderable;

	size_t i = 0;
	Mesh* mesh = model->GetMeshByIndex(i);
	if (mesh == nullptr)
		return;

	Texture* tex = model->GetTextureByMeshIndex(i);
	if (tex != nullptr)
		tex->UseTexture();

	// -- Draw first mesh of each model --
	std::vector<GLObject*> renderableMeshes;
	for (size_t j = 0; j < m_objects.size(); j++) {
		if (RenderMesh(mesh, m_objects[j], filter, uniformModel, shader))
			renderableMeshes.push_back(m_objects[j]);
	}

	// -- If there was no draw call don't continue --
	if (renderableMeshes.size() <= 0)
		return;

	// -- Draw other meshs --
	while(mesh != nullptr) {
		Mesh* mesh = model->GetMeshByIndex(++i);
		if (mesh == nullptr)
			return;

		Texture* tex = model->GetTextureByMeshIndex(i);
		if (tex != nullptr)
			tex->UseTexture();

		for (size_t j = 0; j < renderableMeshes.size(); j++) {
			RenderMesh(mesh, renderableMeshes[j], filter, uniformModel, shader);
		}
	}
}



void GLMeshRenderer::SetRenderable(Mesh * renderable)
{
	m_renderable = renderable;
}

void GLMeshRenderer::Render(RenderFilter filter, GLuint uniformModel, LightedShader* shader)
{
	Mesh* mesh = (Mesh*)m_renderable;
	Texture* tex = mesh->GetTexture();
	if (tex != nullptr)
		tex->UseTexture();

	for (size_t i = 0; i < m_objects.size(); i++) {
		RenderMesh(mesh, m_objects[i], filter, uniformModel, shader);
	}
}


GLCubeMapRenderer::GLCubeMapRenderer()
{
	m_cubemapShader = new CubeMapRenderShader();
	m_cubemapShader->CreateFromFiles("Shaders/envReflection.vert", "Shaders/envReflection.frag", "Shaders/envReflection.geom");

}

void GLCubeMapRenderer::Initialize(Transform* transform) {
	Material* mat = new Material(0.8f, 256, 1.0f, 1.0f, 1.0f);

	Model* mMesh = new Model("Models/Sphere.obj");
	mMesh->Load();
	m_refractModel = new GLModelRenderer();
	m_refractModel->SetRenderable(mMesh);

	m_refractTransform = new Transform(transform);
	m_refractTransform->SetStatic(false);
	m_refractTransform->Scale(0.2f);
	m_refractTransform->Translate(glm::vec3(0.0f, 0.5f, 0.0f));
	m_refractModel->AddMeshRenderer(new GLObject(m_refractTransform, mat, mMesh->GetIndex()));

	mMesh = new Model("Models/Sphere.obj");
	mMesh->Load();
	m_reflectModel = new GLModelRenderer();
	m_reflectModel->SetRenderable(mMesh);


	m_reflectTransform = new Transform(transform);
	m_reflectTransform->SetStatic(false);
	m_reflectTransform->Scale(0.2f);
	m_reflectTransform->Translate(glm::vec3(-5.0f, 1.0f, 0.0f));
	m_reflectModel->AddMeshRenderer(new GLObject(m_reflectTransform, mat, mMesh->GetIndex()));
	
	m_refract = new CubeMap(0.01f, 100.0f);
	m_refract->ReadyCubemap(10.0f);
	m_reflect = new CubeMap(0.01f, 100.0f);
	m_reflect->ReadyCubemap(10.0f);
}

void GLCubeMapRenderer::CubeMapPass(GLRenderer* glRenderer)
{
	if (Camera::GetInstance()->PointInsideViewFrustum(&(m_refractTransform->GetPosition()), 0.0872665f)) {
		float distance = glm::distance(Camera::GetInstance()->GetCameraPosition(), m_refractTransform->GetPosition());
		if (distance < 10.0f)
			m_refract->ReadyCubemap(distance);			
	}
	glRenderer->CubeMapPass(m_refractTransform, m_cubemapShader, m_refract);

	if (Camera::GetInstance()->PointInsideViewFrustum(&(m_reflectTransform->GetPosition()), 0.0872665f)) {
		float distance = glm::distance(Camera::GetInstance()->GetCameraPosition(), m_reflectTransform->GetPosition());
		if (distance < 10.0f)
			m_reflect->ReadyCubemap(distance);
	}
	glRenderer->CubeMapPass(m_reflectTransform, m_cubemapShader, m_reflect);
}

void GLCubeMapRenderer::RenderModels(RenderFilter filter, GLuint uniformModel, LightedShader* shader)
{
	m_refractModel->Render(filter, uniformModel, shader);
	m_reflectModel->Render(filter, uniformModel, shader);
}

void GLCubeMapRenderer::Render(DefaultShader* shader, GLuint uniformModel, GLuint textureUnit)
{
	m_refract->Read(GL_TEXTURE0 + textureUnit);

	shader->SetReflectionFactor(1.0f);
	shader->SetRefractionFactor(1.0f);
	shader->SetFresnelValues(0.0f, 0.9f, 1.0f);
	shader->SetIORValue(0.5f, 0.51f, 0.52f);
	m_refractModel->Render(RenderFilter::R_ALL, uniformModel, shader);

	m_reflect->Read(GL_TEXTURE0 + textureUnit);

	shader->SetReflectionFactor(1.0f);
	shader->SetRefractionFactor(1.0f);
	shader->SetFresnelValues(1.0f, 0.0f, 0.0f);
	m_reflectModel->Render(RenderFilter::R_ALL, uniformModel, shader);
}

GLCubeMapRenderer::~GLCubeMapRenderer()
{
	delete m_reflectModel;
	delete m_refractModel;
	delete m_refract;
	delete m_reflect;
}


GLRenderer::GLRenderer(Transform* transform)
{
	m_pointLightsCount = 0;
	m_spotLightsCount = 0;

	m_directionalSMShader = new DirectionalShadowMapShader();
	m_directionalSMShader->CreateFromFiles("Shaders/dSM.vert", "Shaders/dSM.frag");
	m_omnidirectionalSMShader = new OmnidirectionalShadowMapShader();
	m_omnidirectionalSMShader->CreateFromFiles("Shaders/omniSM.vert", "Shaders/omniSM.frag", "Shaders/omniSM.geom");
	
	m_ambientIntensity = 0.1f;

	std::vector<std::string> faces;
	faces.push_back("Textures/Skybox/miramar_rt.tga");
	faces.push_back("Textures/Skybox/miramar_lf.tga");
	faces.push_back("Textures/Skybox/miramar_up.tga");
	faces.push_back("Textures/Skybox/miramar_dn.tga");
	faces.push_back("Textures/Skybox/miramar_bk.tga");
	faces.push_back("Textures/Skybox/miramar_ft.tga");
	m_skybox = new SkyBox(&faces);

	m_cubemapRenderer = new GLCubeMapRenderer();
	m_cubemapRenderer->Initialize(transform);
}

GLCubeMapRenderer * GLRenderer::GetCubemapRenderer()
{
	return m_cubemapRenderer;
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

Light* GLRenderer::GetPointLightAt(size_t index) {
	if (index >= m_pointLightsCount) {
		return nullptr;
	}
	return m_pointLights[index];
}

Light* GLRenderer::GetSpotLightAt(size_t index) {
	if (index >= m_spotLightsCount) {
		return nullptr;
	}
	return m_spotLights[index];
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

void GLRenderer::AddShader(DefaultShader * shader)
{
	this->m_shader = shader;
}

bool GLRenderer::DynamicMeshes() {
	// Todo: Fix function
	return true;
}

void GLRenderer::RenderScene(RenderFilter filter, GLuint uniformModel, LightedShader* shader) {
	for (size_t i = 0; i < m_renderables.size(); i++)
		m_renderables[i]->Render(filter, uniformModel, shader);
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

	m_cubemapRenderer->RenderModels(filter, uniformModel);

	// Re-bind framebuffer to the default one
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderer::OmnidirectionalSMPass(PointLight* light, RenderFilter filter)
{
	if (filter == RenderFilter::R_ALL || filter == RenderFilter::R_DYNAMIC) {
		printf("Invalid render filter");
		return;
	}

	// Use the directional light shadow map
	m_omnidirectionalSMShader->UseShader();

	ShadowMap* shadowMap = light->GetStaticShadowMap();

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

	m_cubemapRenderer->RenderModels(filter, uniformModel);

	// Re-bind framebuffer to the default one
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderer::CubeMapPass(Transform * transport, CubeMapRenderShader* shader, CubeMap * cubemap)
{
	// Use the directional light shadow map
	shader->UseShader();
	
	// Set viewport to be the directional light shadow map
	glViewport(0, 0, cubemap->GetShadowWidth(), cubemap->GetShadowHeight());

	// Bind framebuffer to be the shadow map texture
	cubemap->Write();

	// Clear buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set uniforms
	shader->SetViewProjectMatrices(
		transport->GetCubeViewProjectionMatrices(
			cubemap->GetAspect(), 
			cubemap->GetNear(), 
			cubemap->GetFar()));
	shader->SetCameraPosition(&glm::vec3(transport->GetPosition().x, transport->GetPosition().y, transport->GetPosition().z));
	shader->SetAmbientIntensity(m_ambientIntensity);
	shader->SetDirectionalLight(m_directionalLight);

	shader->SetTexutre(1);
	shader->SetPointLights(&m_pointLights[0], m_pointLightsCount, 2, 0);
	shader->SetSpotLights(&m_spotLights[0], m_spotLightsCount, 2 + m_pointLightsCount * 2, m_pointLightsCount);

	GLuint uniformModel = shader->GetModelLocation();

	ShaderCompiler::ValidateProgram(shader->GetShaderID());
	
	// Render scene
	RenderScene(RenderFilter::R_ALL, uniformModel, shader);

	// Re-bind framebuffer to the default one
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderer::RenderPass(RenderFilter filter)
{
	// Clear buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw skybox
	m_skybox->Draw(&Camera::GetInstance()->GetViewMatrix(), &Camera::GetInstance()->GetProjectionMatrix());

	// Use the developed default shader
	m_shader->UseShader();

	// Set uniforms
	m_shader->SetProjectionMatrix(&Camera::GetInstance()->GetProjectionMatrix());
	m_shader->SetViewMatrix(&Camera::GetInstance()->GetViewMatrix());
	m_shader->SetCameraPosition(&Camera::GetInstance()->GetCameraPosition());
	m_shader->SetAmbientIntensity(m_ambientIntensity);
	m_shader->SetReflectionFactor(0.0f);
	m_shader->SetRefractionFactor(0.0f);
	m_shader->SetFresnelValues(0.0f, 0.0f, 0.0f);

	int textureUnit = 1;
	m_shader->SetTexutre(textureUnit);

	textureUnit++;
	m_directionalLight->GetStaticShadowMap()->Read(GL_TEXTURE0 + textureUnit);
	m_shader->SetDirectionalStaticSM(textureUnit);

	textureUnit++;
	m_directionalLight->GetDynamicShadowMap()->Read(GL_TEXTURE0 + textureUnit);
	m_shader->SetDirectionalDynamicSM(textureUnit);

	textureUnit++;
	m_skybox->BindSkybox(textureUnit);
	m_shader->SetSkybox(textureUnit);

	textureUnit++;
	size_t worldReflectionUnit = textureUnit;
	m_shader->SetWorldReflection(worldReflectionUnit);

	textureUnit++;
	m_shader->SetPointLights(&m_pointLights[0], m_pointLightsCount, textureUnit, 0);
	m_shader->SetSpotLights(&m_spotLights[0], m_spotLightsCount, textureUnit + m_pointLightsCount * 2, m_pointLightsCount);
	m_shader->SetDirectionalLightTransform(&m_directionalLight->CalculateLightTransform());
	m_shader->SetDirectionalLight(m_directionalLight);

	GLuint uniformModel = m_shader->GetModelLocation();

	ShaderCompiler::ValidateProgram(m_shader->GetShaderID());
	
	// Render scene
	RenderScene(filter, uniformModel, m_shader);
	
	m_cubemapRenderer->Render(m_shader, uniformModel, worldReflectionUnit);
}

void GLRenderer::Render(GLWindow* glWindow, Transform* root, RenderFilter filter)
{
	if (filter != RenderFilter::R_STATIC && DynamicMeshes()) {
		// Only calculate dynamic shadow map if there are dynamic objects to display
		DirectionalSMPass(RenderFilter::R_DYNAMIC);

		m_cubemapRenderer->CubeMapPass(this);

		glWindow->SetViewport();
	}

	RenderPass(filter);
}

void GLRenderer::BakeStage(GLWindow * glWindow)
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

	m_cubemapRenderer->CubeMapPass(this);

	glWindow->SetViewport();
}

GLRenderer::~GLRenderer()
{
	delete m_shader;
	delete m_omnidirectionalSMShader;
	delete m_directionalSMShader;
	delete m_cubemapRenderer;

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
}