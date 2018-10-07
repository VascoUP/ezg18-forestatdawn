#include "ErrorShader.h"

ErrorShader* ErrorShader::instance = NULL;

ErrorShader::ErrorShader() {
	shaderID = ShaderCompiler::CreateFromString(errVShader, errFShader);
}

ErrorShader* ErrorShader::GetInstance() {
	if (instance == NULL) {
		instance = new ErrorShader();
	}
	return instance;
}

GLuint ErrorShader::GetShaderID() {
	return shaderID;
}

ErrorShader::~ErrorShader() {
	ShaderCompiler::ClearShader(&shaderID);
}
