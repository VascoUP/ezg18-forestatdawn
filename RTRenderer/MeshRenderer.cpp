#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
	mat = new Material(0.8f, 256);
}

std::vector<Texture*> MeshRenderer::GetTextures()
{
	return textures;
}

void MeshRenderer::SetLight(Light * light)
{
	directionalLight = light;
}

void MeshRenderer::AddModels(Renderable * mesh)
{
	models.push_back(mesh);
}

void MeshRenderer::AddTexture(const char* texLocation)
{
	Texture* tex = new Texture(texLocation);
	tex->LoadTexture();
	textures.push_back(tex);
}

void MeshRenderer::AddShader(Shader * shader)
{
	//shaders.push_back(shader);
	this->shader = shader;
}

void MeshRenderer::Render()
{
	GLuint uniformModel = 0, uniformView = 0, uniformProjection = 0, uniformAmbientColor = 0, uniformAmbientIntensity = 0, uniformCameraPosition = 0;
	shader->UseShader();
	uniformModel = shader->GetModelLocation();
	uniformView = shader->GetViewLocation();
	uniformProjection = shader->GetProjectionLocation();
	uniformCameraPosition = shader->GetCameraPositionLocation();

	for (size_t i = 0; i < models.size(); i++) {
		if (!models[i])
			continue;

		directionalLight->UseLight(shader->GetAmbienteIntensityLocation(), shader->GetColorLocation(), shader->GetDirectionalDirectionLocation(), shader->GetDirectionalIntensityLocation());
		mat->UseMaterial(shader->GetSpecularIntensityLocation(), shader->GetShininessLocation());

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(models[i]->GetTransform()->TransformMatrix(true)));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->CalculateViewMatrix()));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->ProjectionMatrix()));
		glUniform3f(uniformCameraPosition, Camera::GetInstance()->GetCameraPosition().x, Camera::GetInstance()->GetCameraPosition().y, Camera::GetInstance()->GetCameraPosition().z);

		models[i]->Render();
	}
}


MeshRenderer::~MeshRenderer()
{
	for (size_t i = 0; i < models.size(); i++) {
		if (!models[i])
			continue;
		delete models[i];
	}

	for (size_t i = 0; i < textures.size(); i++) {
		if (!textures[i])
			continue;
		delete textures[i];
	}

	delete shader;
	delete directionalLight;
}
