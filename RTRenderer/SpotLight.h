#pragma once

#include "PointLight.h"

class SpotLight : protected PointLight
{
private:
	glm::vec3 direction;
	GLfloat edge;
	GLfloat	procEdge;

public:
	SpotLight(Transform* transform);
	SpotLight(Transform* transform, GLfloat edge, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);

	void UseLight(GLuint directionLocation, GLuint edgeLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);

	~SpotLight();
};

