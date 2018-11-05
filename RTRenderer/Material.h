#pragma once

#include <GL\glew.h>

class Material
{
private:
	GLfloat specularIntensity;
	GLfloat shininess;

public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shininess);

	void UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation);

	~Material();
};