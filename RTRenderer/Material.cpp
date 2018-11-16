#include "Material.h"

Material::Material()
{
	specularIntensity = 0;
	shininess = 0;
	albedoRed = 1.0f;
	albedoGreen = 1.0f;
	albedoBlue = 1.0f;
	albedoAlpha = 1.0f;
}

Material::Material(GLfloat specularIntensity, GLfloat shininess, GLfloat red, GLfloat green, GLfloat blue)
{
	this->specularIntensity = specularIntensity;
	this->shininess = shininess;
	albedoRed = red;
	albedoGreen = green;
	albedoBlue = blue;
	albedoAlpha = 1.0f;
}

void Material::UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation, unsigned int albedoLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
	glUniform3f(albedoLocation, albedoRed, albedoGreen, albedoBlue);
}


Material::~Material()
{
}
