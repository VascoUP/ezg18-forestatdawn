#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
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
