#include "GLRenderer.h"

Transform* refractTransform;
Transform* reflectTransform;
CubeMap* refract;
CubeMap* reflect;

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

	// -- Draw first mesh of each model --
	std::vector<GLObject*> renderableMeshes;
	for (size_t j = 0; j < m_objects.size(); j++) {
		if (RenderMesh(mesh, m_objects[j], filter, uniformModel))
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
	m_pointLightsCount = 0;
	m_spotLightsCount = 0;

	m_directionalSMShader = new DirectionalShadowMapShader();
	m_directionalSMShader->CreateFromFiles("Shaders/dSM.vert", "Shaders/dSM.frag");
	m_omnidirectionalSMShader = new OmnidirectionalShadowMapShader();
	m_omnidirectionalSMShader->CreateFromFiles("Shaders/omniSM.vert", "Shaders/omniSM.frag", "Shaders/omniSM.geom");
	m_cubemapShader = new CubeMapRenderShader();
	m_cubemapShader->CreateFromFiles("Shaders/envReflection.vert", "Shaders/envReflection.frag", "Shaders/envReflection.geom");
	
	m_material = new Material(0.8f, 256, 1.0f, 1.0f, 1.0f);
	m_ambientIntensity = 0.1f;

	std::vector<std::string> faces;
	faces.push_back("Textures/Skybox/miramar_rt.tga");
	faces.push_back("Textures/Skybox/miramar_lf.tga");
	faces.push_back("Textures/Skybox/miramar_up.tga");
	faces.push_back("Textures/Skybox/miramar_dn.tga");
	faces.push_back("Textures/Skybox/miramar_bk.tga");
	faces.push_back("Textures/Skybox/miramar_ft.tga");
	skybox = new SkyBox(&faces);
}

void GLRenderer::Initialize(Transform* transform) {
	Model* mMesh = new Model("Models/Sphere.obj");
	mMesh->Load();
	refractModel = new GLModelRenderer();
	refractModel->SetRenderable(mMesh);

	refractTransform = new Transform(transform);
	refractTransform->Scale(0.2f);
	refractTransform->Translate(glm::vec3(0.0f, 0.5f, 0.0f));
	refractModel->AddMeshRenderer(new GLObject(refractTransform, mMesh->GetIndex()));

	mMesh = new Model("Models/Sphere.obj");
	mMesh->Load();
	reflectModel = new GLModelRenderer();
	reflectModel->SetRenderable(mMesh);

	reflectTransform = new Transform(transform);
	reflectTransform->Scale(0.2f);
	reflectTransform->Translate(glm::vec3(-5.0f, 1.0f, 0.0f));
	reflectModel->AddMeshRenderer(new GLObject(reflectTransform, mMesh->GetIndex()));

	refract = new CubeMap(0.01f, 100.0f);
	refract->ReadyCubemap(glm::distance(Camera::GetInstance()->GetCameraPosition(), refractTransform->GetPosition()));
	reflect = new CubeMap(0.01f, 100.0f);
	reflect->ReadyCubemap(glm::distance(Camera::GetInstance()->GetCameraPosition(), refractTransform->GetPosition()));
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

	refractModel->Render(RenderFilter::R_ALL, uniformModel);
	reflectModel->Render(RenderFilter::R_ALL, uniformModel);

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

	refractModel->Render(RenderFilter::R_ALL, uniformModel);
	reflectModel->Render(RenderFilter::R_ALL, uniformModel);

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

	// Clear buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set uniforms
	m_cubemapShader->SetViewProjectMatrices(
		transport->GetCubeViewProjectionMatrices(
			cubemap->GetAspect(), 
			cubemap->GetNear(), 
			cubemap->GetFar()));
	m_cubemapShader->SetCameraPosition(&glm::vec3(transport->GetPosition().x, transport->GetPosition().y, transport->GetPosition().z));
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
	// Clear buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw skybox
	skybox->Draw(&Camera::GetInstance()->GetViewMatrix(), &Camera::GetInstance()->GetProjectionMatrix());

	// Use the developed default shader
	m_shader->UseShader();

	// Set uniforms
	m_shader->SetProjectionMatrix(&Camera::GetInstance()->GetProjectionMatrix());
	m_shader->SetViewMatrix(&Camera::GetInstance()->GetViewMatrix());
	m_shader->SetCameraPosition(&Camera::GetInstance()->GetCameraPosition());
	m_shader->SetAmbientIntensity(m_ambientIntensity);
	m_shader->SetMaterial(m_material);
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
	size_t worldReflectionUnit = textureUnit;
	refract->Read(GL_TEXTURE0 + textureUnit);
	m_shader->SetWorldReflection(textureUnit);

	textureUnit++;
	m_shader->SetPointLights(&m_pointLights[0], m_pointLightsCount, textureUnit, 0);
	m_shader->SetSpotLights(&m_spotLights[0], m_spotLightsCount, textureUnit + m_pointLightsCount, m_pointLightsCount);
	m_shader->SetDirectionalLightTransform(&m_directionalLight->CalculateLightTransform());
	m_shader->SetDirectionalLight(m_directionalLight);

	GLuint uniformModel = m_shader->GetModelLocation();

	ShaderCompiler::ValidateProgram(m_shader->GetShaderID());
	
	// Render scene
	RenderScene(filter, uniformModel);

	m_shader->SetReflectionFactor(1.0f);
	m_shader->SetRefractionFactor(1.0f);
	m_shader->SetFresnelValues(0.0f, 0.9f, 1.0f);
	m_shader->SetIORValue(0.5f, 0.51f, 0.52f);
	refractModel->Render(RenderFilter::R_ALL, uniformModel);

	reflect->Read(GL_TEXTURE0 + worldReflectionUnit);

	m_shader->SetReflectionFactor(1.0f);
	m_shader->SetRefractionFactor(1.0f);
	m_shader->SetFresnelValues(1.0f, 0.0f, 0.0f);
	reflectModel->Render(RenderFilter::R_ALL, uniformModel);
}

void GLRenderer::Render(GLWindow* glWindow, Transform* root, RenderFilter filter)
{
	if (filter != RenderFilter::R_STATIC && DynamicMeshes()) {
		// Only calculate dynamic shadow map if there are dynamic objects to display
		DirectionalSMPass(RenderFilter::R_DYNAMIC);

		if (Camera::GetInstance()->PointInsideViewFrustum(&(refractTransform->GetPosition()), 0.0872665f)) {
			float distance = glm::distance(Camera::GetInstance()->GetCameraPosition(), refractTransform->GetPosition());
			if (distance < 10.0f) {
				refract->ReadyCubemap(distance);
				CubeMapPass(refractTransform, refract);
			}
		}
		if (Camera::GetInstance()->PointInsideViewFrustum(&(reflectTransform->GetPosition()), 0.0872665f)) {
			float distance = glm::distance(Camera::GetInstance()->GetCameraPosition(), refractTransform->GetPosition());
			if (distance < 10.0f) {
				reflect->ReadyCubemap(distance);
				CubeMapPass(reflectTransform, reflect);
			}
		}

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

	CubeMapPass(refractTransform, refract);
	CubeMapPass(reflectTransform, reflect);

	glWindow->SetViewport();
}

GLRenderer::~GLRenderer()
{
	delete m_shader;
	delete m_omnidirectionalSMShader;
	delete m_directionalSMShader;
	delete m_cubemapShader;

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

	delete refract;
	delete reflect;
	delete refractModel;
	delete reflectModel;
}