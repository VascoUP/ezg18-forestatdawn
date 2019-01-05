#include "PointLight.h"

PointLight::PointLight(Transform* transform)
	: Light(transform)
{
	constant = 0;
	linear = 0;
	exponent = 0;
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

PointLight::~PointLight()
{
}
