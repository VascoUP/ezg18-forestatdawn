#include "DirectionalShadowMapShader.h"



DirectionalShadowMapShader::DirectionalShadowMapShader()
{
	uniformModel = 0;
	uniformDirectionalLightTransform = 0;
}

void DirectionalShadowMapShader::GetShaderUniforms()
{
	uniformModel = GetUniformLocation("u_modelMatrix");
	uniformDirectionalLightTransform = GetUniformLocation("u_directionalLightTransform");
}

GLuint DirectionalShadowMapShader::GetModelLocation()
{
	return uniformModel;
}

void DirectionalShadowMapShader::SetModel(glm::mat4 * mMatrix)
{
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(*mMatrix));
}

void DirectionalShadowMapShader::SetDirectionalLightTransform(glm::mat4 * lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}