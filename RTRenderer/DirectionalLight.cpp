#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Transform* transform)
	: Light(transform)
{}


DirectionalLight::DirectionalLight(Transform* transform, GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue)
	: Light(transform, diffIntensity, diffRed, diffGreen, diffBlue, specIntensity, specRed, specGreen, specBlue)
{}

void DirectionalLight::UseLight(GLuint directionLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glm::vec3 dir = glm::vec3(transform->GetUp());
	dir = glm::normalize(-dir);
	glUniform3f(directionLocation, dir.x, dir.y, dir.z);
	Light::UseLight(diffuseColorLocation, diffuseFactorLocation, specularColorLocation, specularFactorLocation);
}

DirectionalLight::~DirectionalLight()
{
}
