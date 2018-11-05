#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
	uniformLightColor = 0;
	uniformAmbientIntensity = 0;
	uniformDirectionalIntensity = 0;
	uniformDirectionalDirection = 0;
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
		uniformModel = glGetUniformLocation(shaderID, "model");
		uniformView = glGetUniformLocation(shaderID, "view");
		uniformProjection = glGetUniformLocation(shaderID, "projection");
		uniformProjection = glGetUniformLocation(shaderID, "projection");
		uniformProjection = glGetUniformLocation(shaderID, "projection");
		uniformLightColor = glGetUniformLocation(shaderID, "directionalLight.color");
		uniformCameraPosition = glGetUniformLocation(shaderID, "cameraPosition");
		uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
		uniformDirectionalIntensity = glGetUniformLocation(shaderID, "directionalLight.intensity");
		uniformDirectionalDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
		uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
		uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
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

GLuint Shader::GetColorLocation()
{
	return uniformLightColor;
}

GLuint Shader::GetDirectionalIntensityLocation()
{
	return uniformDirectionalIntensity;
}

GLuint Shader::GetDirectionalDirectionLocation()
{
	return uniformDirectionalDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
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
