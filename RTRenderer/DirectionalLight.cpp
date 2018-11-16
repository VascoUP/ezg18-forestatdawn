#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() 
	: Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}


DirectionalLight::DirectionalLight(GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffIntensity, GLfloat diffRed, GLfloat diffGreen, GLfloat diffBlue, GLfloat specIntensity, GLfloat specRed, GLfloat specGreen, GLfloat specBlue)
	: Light(diffIntensity, diffRed, diffGreen, diffBlue, specIntensity, specRed, specGreen, specBlue)
{
	direction = glm::vec3(xDir, yDir, zDir);

	//glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

void DirectionalLight::UseLight(GLuint directionLocation, GLuint diffuseColorLocation, GLuint diffuseFactorLocation, GLuint specularColorLocation, GLuint specularFactorLocation)
{
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	Light::UseLight(diffuseColorLocation, diffuseFactorLocation, specularColorLocation, specularFactorLocation);
}

DirectionalLight::~DirectionalLight()
{
}
