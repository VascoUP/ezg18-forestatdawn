#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	pointLightCount = 0;

	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
	uniformCameraPosition = 0;
	uniformAmbientIntensity = 0;
	uniformDirectionalLight.uniformDiffuseColor = 0;
	uniformDirectionalLight.uniformDiffuseFactor = 0;
	uniformDirectionalLight.uniformSpecularColor = 0;
	uniformDirectionalLight.uniformSpecularFactor = 0;
	uniformDirectionalLight.uniformDirection = 0;
	uniformMaterial.uniformSpecularIntensity = 0;
	uniformMaterial.uniformShininess = 0;
	uniformMaterial.uniformAlbedo = 0;
	uniformPointLightCount = 0;
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
		// Setup the model and projection matrices
		uniformModel = glGetUniformLocation(shaderID, "u_modelMatrix");
		uniformView = glGetUniformLocation(shaderID, "u_viewMatrix");
		uniformProjection = glGetUniformLocation(shaderID, "u_projectionMatrix");

		uniformCameraPosition = glGetUniformLocation(shaderID, "u_cameraPosition");
		uniformAmbientIntensity = glGetUniformLocation(shaderID, "u_ambientFactor");
		uniformDirectionalLight.uniformDiffuseColor = glGetUniformLocation(shaderID, "u_directionalLight.light.diffuseColor");
		uniformDirectionalLight.uniformDiffuseFactor = glGetUniformLocation(shaderID, "u_directionalLight.light.diffuseFactor");
		uniformDirectionalLight.uniformSpecularColor = glGetUniformLocation(shaderID, "u_directionalLight.light.specularColor");
		uniformDirectionalLight.uniformSpecularFactor = glGetUniformLocation(shaderID, "u_directionalLight.light.specularFactor");
		uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "u_directionalLight.direction");
		uniformMaterial.uniformSpecularIntensity = glGetUniformLocation(shaderID, "u_material.specularIntensity");
		uniformMaterial.uniformShininess = glGetUniformLocation(shaderID, "u_material.shininess");
		uniformMaterial.uniformAlbedo = glGetUniformLocation(shaderID, "u_material.albedo");

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

void Shader::SetPointLights(PointLight * pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
		lightCount = MAX_POINT_LIGHTS;
	glUniform1i(uniformPointLightCount, lightCount);
	for (size_t i = 0; i < lightCount; i++) {
		pLight[i].UseLight(uniformPointLights[i].uniformPosition, uniformPointLights[i].uniformConstant,
			uniformPointLights[i].uniformLinear, uniformPointLights[i].uniformExponent, uniformPointLights[i].uniformDiffuseColor,
			uniformPointLights[i].uniformDiffuseFactor, uniformPointLights[i].uniformSpecularColor, uniformPointLights[i].uniformSpecularFactor);
	}
}

void Shader::SetMaterial(Material * mat)
{
	mat->UseMaterial(uniformMaterial.uniformSpecularIntensity, uniformMaterial.uniformShininess, uniformMaterial.uniformAlbedo);
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
