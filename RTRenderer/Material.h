#pragma once

#include <GL\glew.h>

class Material
{
private:
	GLfloat specularIntensity;
	GLfloat shininess;
	GLfloat albedoRed;
	GLfloat albedoGreen;
	GLfloat albedoBlue;
	GLfloat albedoAlpha;

public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shininess, GLfloat red, GLfloat green, GLfloat blue);

	void UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation, unsigned int albedoLocation);

	~Material();
};