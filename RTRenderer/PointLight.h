#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Light.h"
#include "ShadowMap.h"

class PointLight : public Light
{
protected:
	GLfloat constant;
	GLfloat linear;
	GLfloat exponent;

	GLfloat farPlane;
public:
	PointLight(Transform* transform);
	PointLight(Transform* transform,
		GLfloat near, GLfloat far,
		GLuint staticShadowWidth, GLuint staticShadowHeight,
		GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
		GLfloat constant, GLfloat linear, GLfloat exponent, 
		GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, 
		GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);

	GLfloat GetConstant() const;
	GLfloat GetLinear() const;
	GLfloat GetExponent() const;
	GLfloat GetFarPlane() const;

	void UseLight(GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);

	std::vector<glm::mat4> CalculateLightTransform();

	~PointLight();
};