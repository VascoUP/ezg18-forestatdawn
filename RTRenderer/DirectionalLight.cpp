#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Transform* transform)
	: Light(transform)
{
	GLfloat zFar = 40.0f;
	GLfloat zNear = 0.1f;

	glm::vec3 dir = glm::vec3(transform->GetUp());
	dir = glm::normalize(dir);
	lightProj = glm::ortho(-zFar, zFar, -zFar, zFar, zNear, zFar) *
		glm::lookAt(dir * zFar / 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}


DirectionalLight::DirectionalLight(
	Transform* transform,
	GLuint staticShadowWidth, GLuint staticShadowHeight,
	GLuint dynamicShadowWidth, GLuint dynamicShadowHeight,
	GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, 
	GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue)
	: Light(transform, 
		staticShadowWidth, staticShadowHeight,
		dynamicShadowWidth, dynamicShadowHeight, 
		diffIntensity, diffRed, diffGreen, diffBlue, specIntensity, specRed, specGreen, specBlue)
{
	GLfloat zFar = 100.0f;
	GLfloat zNear = 0.1f;

	glm::vec3 dir = glm::vec3(transform->GetUp());
	dir = glm::normalize(dir);
	lightProj = glm::ortho(-zFar, zFar, -zFar, zFar, zNear, zFar) *
		glm::lookAt(dir * zFar / 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 GetDirection(glm::vec3 upVector) {
	glm::vec3 dir = glm::vec3(upVector);
	return glm::normalize(-dir);
}

void DirectionalLight::UseLight(GLuint directionLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glm::vec3 dir = glm::vec3(transform->GetUp());
	dir = glm::normalize(-dir);
	glUniform3f(directionLocation, dir.x, dir.y, dir.z);
	Light::UseLight(diffuseColorLocation, diffuseFactorLocation, specularColorLocation, specularFactorLocation);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj;
}

DirectionalLight::~DirectionalLight()
{
}
