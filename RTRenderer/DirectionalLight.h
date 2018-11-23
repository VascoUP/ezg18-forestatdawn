#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Light.h"

class DirectionalLight : protected Light
{
public:
	DirectionalLight(Transform* transform);
	DirectionalLight(Transform* transform, GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);
	
	void UseLight(GLuint directionLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);
	
	~DirectionalLight();
};