#include "SpotLight.h"

SpotLight::SpotLight(Transform* transform)
	: PointLight(transform)
{
	edge = 0;
	procEdge = 0;
}

SpotLight::SpotLight(Transform* transform,
	GLfloat near, GLfloat far,
	GLuint staticShadowWidth, GLuint staticShadowHeight,
	GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
	GLfloat edge, GLfloat constant, GLfloat linear, GLfloat exponent,
	GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue,
	GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue): 
	PointLight(transform, 
		far, near,
		staticShadowWidth, staticShadowHeight,
		dynamicShadowWidth, dynamicShadowHeight,
		constant, linear, exponent, 
		diffIntensity, diffRed, diffGreen, diffBlue, 
		specIntensity, specRed, specGreen, specBlue)
{
	this->edge = edge;
	procEdge = edge;
}

GLfloat SpotLight::GetEdge() const
{
	return edge;
}

GLfloat SpotLight::GetProcEdge() const
{
	return procEdge;
}

void SpotLight::UseLight(GLuint directionLocation, GLuint edgeLocation, 
	GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, 
	GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glm::vec3 dir = glm::normalize(-transform->GetUp());
	glUniform3f(directionLocation, dir.x, dir.y, dir.z);
	glUniform1f(edgeLocation, procEdge);
	PointLight::UseLight(positionLocation, constantLocation, linearLocation, exponentLocation, diffuseColorLocation, diffuseFactorLocation, specularColorLocation, specularFactorLocation);
}

SpotLight::~SpotLight()
{
}
