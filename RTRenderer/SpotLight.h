#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
private:
	GLfloat edge;
	GLfloat	procEdge;

public:
	SpotLight(Transform* transform);
	SpotLight(Transform* transform,
		GLfloat near, GLfloat far,
		GLuint staticShadowWidth, GLuint staticShadowHeight,
		GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
		GLfloat edge, GLfloat constant, GLfloat linear, GLfloat exponent, 
		GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, 
		GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);

	GLfloat GetEdge() const;
	GLfloat GetProcEdge() const;

	void UseLight(GLuint directionLocation, GLuint edgeLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);

	~SpotLight();
};

