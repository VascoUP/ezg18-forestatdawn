#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{
protected:
	GLfloat diffuseIntensity;
	glm::vec3 diffuseColor;
	GLfloat specularIntensity;
	glm::vec3 specularColor;

public:
	Light();
	Light(GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);

	void UseLight(GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);

	~Light();
};

