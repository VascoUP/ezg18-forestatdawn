#pragma once

#include <stdio.h>
#include <string>

#include <GL\glew.h>

// This class is used to preform the initialization and destruction of any shader program
class ShaderCompiler
{
public:
	// Creates a shader program from the vertex and fragment shader's code
	static GLuint CreateStandardShader(const char* vertexCode, const char* fragmentCode);
	//
	static GLuint CreateSingleShader(const char* code, GLenum shaderType);
	// Fress the memory allocated by given shader program
	static void ClearShader(GLuint* shaderID);
private:
	// Compiler the shader program and handle possible shader errors
	static GLuint CompileShader(const char* vertexCode, const char* fragmentCode);
	//
	static bool ValidateProgram(GLuint myProgram);
	// Add a given shader code of a certain type to a shader program
	static bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

