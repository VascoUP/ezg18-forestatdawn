#pragma once

#include "Shader.h"

class DirectionalShadowMapShader :
	public Shader
{
private:
	GLuint uniformModel;
	GLuint uniformDirectionalLightTransform;
public:
	DirectionalShadowMapShader();

	GLuint GetModelLocation();

	void SetModel(glm::mat4* mMatrix);
	void SetDirectionalLightTransform(glm::mat4* lTransform);

protected:
	void GetShaderUniforms();
};

