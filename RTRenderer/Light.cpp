#include "Light.h"

Light::Light(Transform * transform,
	GLuint staticShadowWidth, GLuint staticShadowHeight,
	GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
	GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, 
	GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue)
{
	this->transform = transform;

	diffuseColor = glm::vec3(diffRed, diffGreen, diffBlue);
	diffuseIntensity = diffIntensity;

	specularColor = glm::vec3(specRed, specGreen, specBlue);
	specularIntensity = specIntensity;

	m_staticSM = new ShadowMap();
	
	m_staticSM = new ShadowMap();
	m_staticSM->Init(staticShadowWidth, staticShadowHeight);
}

Transform * Light::GetTransform() const
{
	return transform;
}

GLfloat Light::GetDiffuseIntensity() const
{
	return diffuseIntensity;
}

glm::vec3 Light::GetDiffuseColor() const
{
	return diffuseColor;
}

GLfloat Light::GetSpecularIntensity() const
{
	return specularIntensity;
}

glm::vec3 Light::GetSpecularColor() const
{
	return specularColor;
}

ShadowMap * Light::GetStaticShadowMap()
{
	return m_staticSM;
}

bool Light::IsActive() const {
	return isActive;
}

void Light::SetActive(bool active) {
	isActive = active;
}

void Light::UseLight(GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	if (!isActive) {
		glUniform3f(diffuseColorLocation, 0, 0, 0);
		glUniform3f(diffuseFactorLocation, 0, 0, 0);
		glUniform3f(specularColorLocation, 0, 0, 0);
		glUniform3f(specularFactorLocation, 0, 0, 0);
		return;
	}
	glUniform3f(diffuseColorLocation, diffuseColor.r, diffuseColor.g, diffuseColor.b);
	glUniform3f(diffuseFactorLocation, diffuseIntensity, diffuseIntensity, diffuseIntensity);
	glUniform3f(specularColorLocation, specularColor.r, specularColor.g, specularColor.b);
	glUniform3f(specularFactorLocation, specularIntensity, specularIntensity, specularIntensity);
}


DirectionalLight::DirectionalLight(
	Transform* transform,
	GLuint staticShadowWidth, GLuint staticShadowHeight,
	GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
	GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue,
	GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue)
	: Light(transform,
		staticShadowWidth, staticShadowHeight,
		dynamicShadowWidth, dynamicShadowHeight,
		diffIntensity, diffRed, diffGreen, diffBlue, specIntensity, specRed, specGreen, specBlue)
{
	m_dynamicSM = new ShadowMap();
	m_dynamicSM->Init(dynamicShadowWidth, dynamicShadowHeight);

	GLfloat zFar = 100.0f;
	GLfloat zNear = 0.1f;

	glm::vec3 dir = glm::vec3(transform->GetUp());
	dir = glm::normalize(dir);
	lightProj = glm::ortho(-zFar, zFar, -zFar, zFar, zNear, zFar) *
		glm::lookAt(dir * zFar / 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

ShadowMap * DirectionalLight::GetDynamicShadowMap()
{
	return m_dynamicSM;
}

glm::vec3 GetDirection(glm::vec3 upVector) {
	glm::vec3 dir = glm::vec3(upVector);
	return glm::normalize(-dir);
}

void DirectionalLight::UseLight(GLuint directionLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glm::vec3 dir = glm::vec3(transform->GetUp());
	dir = glm::normalize(-dir);
	glUniform3f(directionLocation, dir.x, dir.y, dir.z);
	Light::UseLight(diffuseColorLocation, diffuseFactorLocation, specularColorLocation, specularFactorLocation);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj;
}


PointLight::PointLight(Transform* transform,
	GLfloat near, GLfloat far,
	GLuint staticShadowWidth, GLuint staticShadowHeight,
	GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
	GLfloat constant, GLfloat linear, GLfloat exponent,
	GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue,
	GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue) :
	Light(transform,
		staticShadowWidth, staticShadowHeight,
		dynamicShadowWidth, dynamicShadowHeight,
		diffIntensity, diffRed, diffGreen, diffBlue,
		specIntensity, specRed, specGreen, specBlue)
{
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;
	this->farPlane = far;

	float aspect = (float)staticShadowWidth / (float)staticShadowHeight;
	lightProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	m_staticSM = new OmniShadowMap();
	m_staticSM->Init(staticShadowWidth, staticShadowHeight);
}

GLfloat PointLight::GetConstant() const
{
	return constant;
}

GLfloat PointLight::GetLinear() const
{
	return linear;
}

GLfloat PointLight::GetExponent() const
{
	return exponent;
}

GLfloat PointLight::GetFarPlane() const
{
	return farPlane;
}

void PointLight::UseLight(GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glm::vec3 position = transform->GetPosition();
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
	Light::UseLight(diffuseColorLocation, diffuseFactorLocation, specularColorLocation, specularFactorLocation);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	glm::vec3 position = transform->GetPosition();
	std::vector<glm::mat4> lightTransforms;
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	return lightTransforms;
}


SpotLight::SpotLight(Transform* transform,
	GLfloat near, GLfloat far,
	GLuint staticShadowWidth, GLuint staticShadowHeight,
	GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
	GLfloat edge, GLfloat constant, GLfloat linear, GLfloat exponent,
	GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue,
	GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue) :
	PointLight(transform,
		far, near,
		staticShadowWidth, staticShadowHeight,
		dynamicShadowWidth, dynamicShadowHeight,
		constant, linear, exponent,
		diffIntensity, diffRed, diffGreen, diffBlue,
		specIntensity, specRed, specGreen, specBlue)
{
	this->edge = edge;
	procEdge = edge;
}

GLfloat SpotLight::GetEdge() const
{
	return edge;
}

GLfloat SpotLight::GetProcEdge() const
{
	return procEdge;
}

void SpotLight::UseLight(GLuint directionLocation, GLuint edgeLocation,
	GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
	GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glm::vec3 dir = glm::normalize(-transform->GetUp());
	glUniform3f(directionLocation, dir.x, dir.y, dir.z);
	glUniform1f(edgeLocation, procEdge);
	PointLight::UseLight(positionLocation, constantLocation, linearLocation, exponentLocation, diffuseColorLocation, diffuseFactorLocation, specularColorLocation, specularFactorLocation);
}
