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
	Texture* normalMap;

public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shininess, GLfloat red, GLfloat green, GLfloat blue);

	void SetAlbedo(Texture* albedo);
	void SetNormalMap(Texture* normalMap);
	void UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation, unsigned int albedoLocation);

	~Material();
};