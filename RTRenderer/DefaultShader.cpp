#include "DefaultShader.h"

DefaultShader::DefaultShader()
{
	pointLightCount = 0;
	spotLightCount = 0;

	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
	uniformCameraPosition = 0;
	uniformTexture = 0;
	uniformAmbientIntensity = 0;
	uniformDirectionalLight.uniformDiffuseColor = 0;
	uniformDirectionalLight.uniformDiffuseFactor = 0;
	uniformDirectionalLight.uniformSpecularColor = 0;
	uniformDirectionalLight.uniformSpecularFactor = 0;
	uniformDirectionalLight.uniformDirection = 0;
	uniformDirectionalStaticSM = 0;
	uniformMaterial.uniformSpecularIntensity = 0;
	uniformMaterial.uniformShininess = 0;
	uniformMaterial.uniformAlbedo = 0;
	uniformPointLightCount = 0;
	uniformSpotLightCount = 0;
}

void DefaultShader::GetShaderUniforms()
{
	uniformModel = GetUniformLocation("u_modelMatrix");
	uniformView = GetUniformLocation("u_viewMatrix");
	uniformProjection = GetUniformLocation("u_projectionMatrix");

	uniformCameraPosition = GetUniformLocation("u_cameraPosition");
	uniformAmbientIntensity = GetUniformLocation("u_ambientFactor");

	// -- Material Uniforms -- 
	uniformMaterial.uniformSpecularIntensity = GetUniformLocation("u_material.specularIntensity");
	uniformMaterial.uniformShininess = GetUniformLocation("u_material.shininess");
	uniformMaterial.uniformAlbedo = GetUniformLocation("u_material.albedo");

	// -- Directional Light Uniforms --
	uniformDirectionalLight.uniformDiffuseColor = GetUniformLocation("u_directionalLight.light.diffuseColor");
	uniformDirectionalLight.uniformDiffuseFactor = GetUniformLocation("u_directionalLight.light.diffuseFactor");
	uniformDirectionalLight.uniformSpecularColor = GetUniformLocation("u_directionalLight.light.specularColor");
	uniformDirectionalLight.uniformSpecularFactor = GetUniformLocation("u_directionalLight.light.specularFactor");
	uniformDirectionalLight.uniformDirection = GetUniformLocation("u_directionalLight.direction");

	// -- Point Lights Uniforms --
	uniformPointLightCount = GetUniformLocation("u_pointLightsCount");
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
		char locBuff[100] = { "\0" };
		snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].light.diffuseColor", i);
		uniformPointLights[i].uniformDiffuseColor = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].light.diffuseFactor", i);
		uniformPointLights[i].uniformDiffuseFactor = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].light.specularColor", i);
		uniformPointLights[i].uniformSpecularColor = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].light.specularFactor", i);
		uniformPointLights[i].uniformSpecularFactor = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].position", i);
		uniformPointLights[i].uniformPosition = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].constant", i);
		uniformPointLights[i].uniformConstant = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].linear", i);
		uniformPointLights[i].uniformLinear = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].exponent", i);
		uniformPointLights[i].uniformExponent = GetUniformLocation(locBuff);
	}

	// -- Spot Light Uniforms --
	uniformSpotLightCount = GetUniformLocation("u_spotLightsCount");
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
		char locBuff[100] = { "\0" };
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.light.diffuseColor", i);
		uniformSpotLights[i].uniformDiffuseColor = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.light.diffuseFactor", i);
		uniformSpotLights[i].uniformDiffuseFactor = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.light.specularColor", i);
		uniformSpotLights[i].uniformSpecularColor = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.light.specularFactor", i);
		uniformSpotLights[i].uniformSpecularFactor = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.position", i);
		uniformSpotLights[i].uniformPosition = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.constant", i);
		uniformSpotLights[i].uniformConstant = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.linear", i);
		uniformSpotLights[i].uniformLinear = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.exponent", i);
		uniformSpotLights[i].uniformExponent = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].direction", i);
		uniformSpotLights[i].uniformDirection = GetUniformLocation(locBuff);
		snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].edge", i);
		uniformSpotLights[i].uniformEdge = GetUniformLocation(locBuff);
	}


	uniformTexture = GetUniformLocation("u_mainTexture");
	uniformDirectionalStaticSM = GetUniformLocation("u_directionalStaticSM");
	uniformDirectionalDynamicSM = GetUniformLocation("u_directionalDynamicSM");
}

GLuint DefaultShader::GetModelLocation() {
	return uniformModel;
}

void DefaultShader::SetProjectionMatrix(glm::mat4 * pMatrix)
{
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(*pMatrix));
}

void DefaultShader::SetViewMatrix(glm::mat4 * vMatrix)
{
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(*vMatrix));
}

void DefaultShader::SetCameraPosition(glm::vec3 * cPosition)
{
	glUniform3f(uniformCameraPosition, cPosition->x, cPosition->y, cPosition->z);
}

void DefaultShader::SetAmbientIntensity(GLfloat aIntensity)
{
	glUniform1f(uniformAmbientIntensity, aIntensity);
}

void DefaultShader::SetDirectionalLight(DirectionalLight * light)
{
	light->UseLight(uniformDirectionalLight.uniformDirection, uniformDirectionalLight.uniformDiffuseColor, uniformDirectionalLight.uniformDiffuseFactor, uniformDirectionalLight.uniformSpecularColor, uniformDirectionalLight.uniformSpecularFactor);
}

void DefaultShader::SetPointLights(PointLight **pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
		lightCount = MAX_POINT_LIGHTS;
	glUniform1i(uniformPointLightCount, lightCount);
	for (size_t i = 0; i < lightCount; i++) {
		pLight[i]->UseLight(uniformPointLights[i].uniformPosition, uniformPointLights[i].uniformConstant,
			uniformPointLights[i].uniformLinear, uniformPointLights[i].uniformExponent, uniformPointLights[i].uniformDiffuseColor,
			uniformPointLights[i].uniformDiffuseFactor, uniformPointLights[i].uniformSpecularColor, uniformPointLights[i].uniformSpecularFactor);
	}
}

void DefaultShader::SetSpotLights(SpotLight **sLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS)
		lightCount = MAX_SPOT_LIGHTS;
	glUniform1i(uniformSpotLightCount, lightCount);
	for (size_t i = 0; i < lightCount; i++) {
		sLight[i]->UseLight(uniformSpotLights[i].uniformDirection, uniformSpotLights[i].uniformEdge,
			uniformSpotLights[i].uniformPosition, uniformSpotLights[i].uniformConstant, uniformSpotLights[i].uniformLinear, uniformSpotLights[i].uniformExponent,
			uniformSpotLights[i].uniformDiffuseColor, uniformSpotLights[i].uniformDiffuseFactor,
			uniformSpotLights[i].uniformSpecularColor, uniformSpotLights[i].uniformSpecularFactor);
	}
}

void DefaultShader::SetMaterial(Material * mat)
{
	mat->UseMaterial(uniformMaterial.uniformSpecularIntensity, uniformMaterial.uniformShininess, uniformMaterial.uniformAlbedo);
}

void DefaultShader::SetTexutre(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void DefaultShader::SetDirectionalStaticSM(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalStaticSM, textureUnit);
}

void DefaultShader::SetDirectionalDynamicSM(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalDynamicSM, textureUnit);
}

void DefaultShader::SetDirectionalLightTransform(glm::mat4 * lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}