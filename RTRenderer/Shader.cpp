#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
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
	uniformDirectionalLightTransform = 0;
	uniformDirectionalSM = 0;
	uniformMaterial.uniformSpecularIntensity = 0;
	uniformMaterial.uniformShininess = 0;
	uniformMaterial.uniformAlbedo = 0;
	uniformPointLightCount = 0;
	uniformSpotLightCount = 0;
}

bool Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
	bool success = true;
	shaderID = ShaderCompiler::CreateFromString(vertexCode, fragmentCode);
	// Fall back on the Error Shader if a problem has happened
	if (!shaderID) {
		ErrorShader* errShader = ErrorShader::GetInstance();
		shaderID = errShader->GetShaderID();
		success = false;
	}

	if (shaderID) {
		uniformModel = glGetUniformLocation(shaderID, "u_modelMatrix");
		uniformView = glGetUniformLocation(shaderID, "u_viewMatrix");
		uniformProjection = glGetUniformLocation(shaderID, "u_projectionMatrix");

		uniformCameraPosition = glGetUniformLocation(shaderID, "u_cameraPosition");
		uniformAmbientIntensity = glGetUniformLocation(shaderID, "u_ambientFactor");

		// -- Material Uniforms -- 
		uniformMaterial.uniformSpecularIntensity = glGetUniformLocation(shaderID, "u_material.specularIntensity");
		uniformMaterial.uniformShininess = glGetUniformLocation(shaderID, "u_material.shininess");
		uniformMaterial.uniformAlbedo = glGetUniformLocation(shaderID, "u_material.albedo");

		// -- Directional Light Uniforms --
		uniformDirectionalLight.uniformDiffuseColor = glGetUniformLocation(shaderID, "u_directionalLight.light.diffuseColor");
		uniformDirectionalLight.uniformDiffuseFactor = glGetUniformLocation(shaderID, "u_directionalLight.light.diffuseFactor");
		uniformDirectionalLight.uniformSpecularColor = glGetUniformLocation(shaderID, "u_directionalLight.light.specularColor");
		uniformDirectionalLight.uniformSpecularFactor = glGetUniformLocation(shaderID, "u_directionalLight.light.specularFactor");
		uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "u_directionalLight.direction");

		// -- Point Lights Uniforms --
		uniformPointLightCount = glGetUniformLocation(shaderID, "u_pointLightsCount");
		for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
			char locBuff[100] = { "\0" };
			snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].light.diffuseColor", i);
			uniformPointLights[i].uniformDiffuseColor = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].light.diffuseFactor", i);
			uniformPointLights[i].uniformDiffuseFactor = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].light.specularColor", i);
			uniformPointLights[i].uniformSpecularColor = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].light.specularFactor", i);
			uniformPointLights[i].uniformSpecularFactor = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].position", i);
			uniformPointLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].constant", i);
			uniformPointLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].linear", i);
			uniformPointLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_pointLights[%d].exponent", i);
			uniformPointLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
		}

		// -- Spot Light Uniforms --
		uniformSpotLightCount = glGetUniformLocation(shaderID, "u_spotLightsCount");
		for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
			char locBuff[100] = { "\0" };
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.light.diffuseColor", i);
			uniformSpotLights[i].uniformDiffuseColor = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.light.diffuseFactor", i);
			uniformSpotLights[i].uniformDiffuseFactor = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.light.specularColor", i);
			uniformSpotLights[i].uniformSpecularColor = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.light.specularFactor", i);
			uniformSpotLights[i].uniformSpecularFactor = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.position", i);
			uniformSpotLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.constant", i);
			uniformSpotLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.linear", i);
			uniformSpotLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].light.exponent", i);
			uniformSpotLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].direction", i);
			uniformSpotLights[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);
			snprintf(locBuff, sizeof(locBuff), "u_spotLights[%d].edge", i);
			uniformSpotLights[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
		}


		uniformTexture = glGetUniformLocation(shaderID, "u_mainTexture");
		uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "u_directionalLightTransform");
		uniformDirectionalSM = glGetUniformLocation(shaderID, "u_directionalSM");
	}

	return success;
}

bool Shader::CreateFromFiles(const char* vertexFile, const char* fragmentFile) {
	bool success = true;
	std::string vertexString = ReadFile(vertexFile);
	std::string fragmentString = ReadFile(fragmentFile);

	if (vertexString == "" || fragmentString == "") {
		ErrorShader* errShader = ErrorShader::GetInstance();
		shaderID = errShader->GetShaderID();
		success = false;
	}

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	return CreateFromString(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* file) {
	std::string code = "";
	std::ifstream fileStream(file, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File does not exist", file);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		code.append(line + "\n");
	}
	fileStream.close();

	return code;
}

GLuint Shader::GetProjectionLocation() {
	return uniformProjection;
}

GLuint Shader::GetViewLocation() {
	return uniformView;
}

GLuint Shader::GetModelLocation() {
	return uniformModel;
}

GLuint Shader::GetCameraPositionLocation()
{
	return uniformCameraPosition;
}

GLuint Shader::GetAmbienteIntensityLocation() {
	return uniformAmbientIntensity;
}

void Shader::SetDirectionalLight(DirectionalLight * light)
{
	light->UseLight(uniformDirectionalLight.uniformDirection, uniformDirectionalLight.uniformDiffuseColor, uniformDirectionalLight.uniformDiffuseFactor, uniformDirectionalLight.uniformSpecularColor, uniformDirectionalLight.uniformSpecularFactor);
}

void Shader::SetPointLights(PointLight **pLight, unsigned int lightCount)
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

void Shader::SetSpotLights(SpotLight **sLight, unsigned int lightCount)
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

void Shader::SetMaterial(Material * mat)
{
	mat->UseMaterial(uniformMaterial.uniformSpecularIntensity, uniformMaterial.uniformShininess, uniformMaterial.uniformAlbedo);
}

void Shader::SetTexutre(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalSM(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalSM, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4 * lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::UseShader() {
	if (!shaderID) {
		printf("No initialized shader has been found");
		return;
	}
	glUseProgram(shaderID);
}

void Shader::ClearShader() {
	ShaderCompiler::ClearShader(&shaderID);

	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	ClearShader();
}
