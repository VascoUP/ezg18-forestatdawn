#include "ShaderCompiler.h"

bool ShaderCompiler::AddShader(GLuint myProgram, const char* shaderCode, GLenum shaderType) {
	GLuint myShader = glCreateShader(shaderType);

	const GLchar* myCode[1];
	myCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = (GLint)strlen(shaderCode);

	glShaderSource(myShader, 1, myCode, codeLength);
	glCompileShader(myShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(myShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(myShader, 1024, NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n'%s'", shaderType, eLog);
		return false;
	}

	glAttachShader(myProgram, myShader);

	return true;
}

GLuint ShaderCompiler::CompileShader(const char* vertexCode, const char* fragmentCode) {
	GLuint shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Error creating shader program");
		return 0;
	}

	if (!AddShader(shaderID, vertexCode, GL_VERTEX_SHADER)) {
		return 0;
	}
	if (!AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER)) {
		return 0;
	}

	GLint result = 0;
	GLchar oLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(oLog), NULL, oLog);
		printf("Error linking program: '%s'\n", oLog);
		return 0;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(oLog), NULL, oLog);
		printf("Error validating program: '%s'\n", oLog);
		return 0;
	}

	return shaderID;
}

GLuint ShaderCompiler::CreateFromString(const char* vertexCode, const char* fragmentCode) {
	return CompileShader(vertexCode, fragmentCode);
}

void ShaderCompiler::ClearShader(GLuint* shaderID) {
	if (!*shaderID) {
		glDeleteProgram(*shaderID);
		shaderID = 0;
	}
}