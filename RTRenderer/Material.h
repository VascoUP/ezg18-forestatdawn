#pragma once

#include <GL\glew.h>

#include "Texture.h"

class Material
{
private:
	GLfloat specularIntensity;
	GLfloat shininess;
	GLfloat albedoRed;
	GLfloat albedoGreen;
	GLfloat albedoBlue;
	GLfloat albedoAlpha;
	Texture* albedo;

public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shininess, GLfloat red, GLfloat green, GLfloat blue, Texture* albedo = nullptr);

	void SetAlbedo(Texture* albedo);
	void UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation, unsigned int albedoLocation);

	~Material();
};