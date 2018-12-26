#include "Light.h"

Light::Light(Transform* transform)
{
	this->transform = transform;

	diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	diffuseIntensity = 0.5f;

	specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	specularIntensity = 0.5f;
}

Light::Light(Transform * transform, GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue)
{
	this->transform = transform;

	diffuseColor = glm::vec3(diffRed, diffGreen, diffBlue);
	diffuseIntensity = diffIntensity;

	specularColor = glm::vec3(specRed, specGreen, specBlue);
	specularIntensity = specIntensity;
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

void Light::UseLight(GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glUniform3f(diffuseColorLocation, diffuseColor.r, diffuseColor.g, diffuseColor.b);
	glUniform3f(diffuseFactorLocation, diffuseIntensity, diffuseIntensity, diffuseIntensity);
	glUniform3f(specularColorLocation, specularColor.r, specularColor.g, specularColor.b);
	glUniform3f(specularFactorLocation, specularIntensity, specularIntensity, specularIntensity);
}

Light::~Light()
{
}
