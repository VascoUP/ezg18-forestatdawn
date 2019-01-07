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

	ShadowMap* m_staticSM;
	ShadowMap* m_dynamicSM;
public:
	Light(Transform* transform);
	Light(Transform* transform,
		GLuint staticShadowWidth, GLuint staticShadowHeight,
		GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
		GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, 
		GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);

	Transform* GetTransform() const;
	GLfloat GetDiffuseIntensity() const;
	glm::vec3 GetDiffuseColor() const;
	GLfloat GetSpecularIntensity() const;
	glm::vec3 GetSpecularColor() const;
	ShadowMap* GetStaticShadowMap();
	ShadowMap* GetDynamicShadowMap();

	void UseLight(GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(Transform* transform);
	DirectionalLight(Transform* transform,
		GLuint staticShadowWidth, GLuint staticShadowHeight,
		GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
		GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue,
		GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue);

	void UseLight(GLuint directionLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation);

	glm::mat4 CalculateLightTransform();
};

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
};

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
};

