#include "Light.h"

Light::Light()
{
	diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	diffuseIntensity = 0.5f;

	specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	specularIntensity = 0.5f;
}

Light::Light(GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue)
{
	diffuseColor = glm::vec3(diffRed, diffGreen, diffBlue);
	diffuseIntensity = diffIntensity;

	specularColor = glm::vec3(specRed, specGreen, specBlue);
	specularIntensity = specIntensity;
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
