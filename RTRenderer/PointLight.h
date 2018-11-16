#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Light.h"

class PointLight : Light
{
private:
	glm::vec3 position;
	GLfloat constant;
	GLfloat linear;
	GLfloat exponent;
public:
	PointLight();
	PointLight(GLfloat xPosition, GLfloat yPosition, GLfloat zPosition, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);

	void UseLight(GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);

	~PointLight();
};

