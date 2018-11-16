#include "PointLight.h"

PointLight::PointLight()
	: Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 0;
	linear = 0;
	exponent = 0;
}

PointLight::PointLight(GLfloat xPosition, GLfloat yPosition, GLfloat zPosition, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue)
	: Light(diffIntensity, diffRed, diffGreen, diffBlue, specIntensity, specRed, specGreen, specBlue)
{
	position = glm::vec3(xPosition, yPosition, zPosition);
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;
}

void PointLight::UseLight(GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
	Light::UseLight(diffuseColorLocation, diffuseFactorLocation, specularColorLocation, specularFactorLocation);
}

PointLight::~PointLight()
{
}
