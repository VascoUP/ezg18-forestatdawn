#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(Transform* transform);
	DirectionalLight(Transform* transform,
		GLfloat staticShadowWidth, GLfloat staticShadowHeight,
		GLfloat dynamicShadowWidth, GLfloat dynamicShadowHeight,
		GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, 
		GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);
	
	void UseLight(GLuint directionLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);
	
	glm::mat4 CalculateLightTransform();

	~DirectionalLight();
};