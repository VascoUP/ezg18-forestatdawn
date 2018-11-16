#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Light.h"

class DirectionalLight : Light
{
private:
	glm::vec3 direction;
public:
	DirectionalLight();
	DirectionalLight(GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);
	
	void UseLight(GLuint directionLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);
	
	~DirectionalLight();
};

