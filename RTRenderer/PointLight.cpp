#include "PointLight.h"

PointLight::PointLight(Transform* transform)
	: Light(transform)
{
	constant = 0;
	linear = 0;
	exponent = 0;
}

PointLight::PointLight(Transform* transform, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue)
	: Light(transform, 1024, 1024, diffIntensity, diffRed, diffGreen, diffBlue, specIntensity, specRed, specGreen, specBlue)
{
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;
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

void PointLight::UseLight(GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glm::vec3 pos = transform->LocalToWorldCoordinates(transform->GetPosition(), Transform::POINT_TYPE::POINT);
	glUniform3f(positionLocation, pos.x, pos.y, pos.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
	Light::UseLight(diffuseColorLocation, diffuseFactorLocation, specularColorLocation, specularFactorLocation);
}

PointLight::~PointLight()
{
}
