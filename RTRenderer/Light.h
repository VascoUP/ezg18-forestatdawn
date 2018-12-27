#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Transform.h"
#include "ShadowMap.h"

class Light {
protected:
	Transform* transform;

	GLfloat diffuseIntensity;
	glm::vec3 diffuseColor;
	GLfloat specularIntensity;
	glm::vec3 specularColor;

	glm::mat4 lightProj;

	ShadowMap* mSM;
public:
	Light(Transform* transform);
	Light(Transform* transform,
		GLfloat shadowWidth, GLfloat shadowHeight,
		GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, 
		GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);

	Transform* GetTransform() const;
	GLfloat GetDiffuseIntensity() const;
	glm::vec3 GetDiffuseColor() const;
	GLfloat GetSpecularIntensity() const;
	glm::vec3 GetSpecularColor() const;
	ShadowMap* GetShadowMap();

	void UseLight(GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);

	~Light();
};

