#pragma once

#include <stdio.h>
#include <GL\glew.h>

#include "ShaderCompiler.h"

// Error Vertex Shader
static const char* errVShader = "									\n\
#version 330														\n\
																	\n\
layout (location = 0) in vec3 pos;									\n\
																	\n\
uniform mat4 model;													\n\
uniform mat4 projection;											\n\
																	\n\
void main() {														\n\
	gl_Position = projection * model * vec4(pos.xyz, 1.0);			\n\
}";

// Error Fragment Shader
static const char* errFShader = "									\n\
#version 330														\n\
																	\n\
out	vec4 colour;													\n\
																	\n\
void main() {														\n\
	colour = vec4(1.0, 0.0, 1.0, 1.0);								\n\
}";

// This class hold the ID of a fallback shader when an error happens
class ErrorShader
{
private:
	// Instance of the Singleton object of this class
	static ErrorShader* instance;
	// ID of the fallback shader
	GLuint shaderID;

	// Private constructor to ensure a singleton pattern
	ErrorShader();
public:
	// This function return the only instance of this class. If it doesn't exist yet, it creates one.
	static ErrorShader* GetInstance();
	// Return the ID of the fallback shader
	GLuint GetShaderID();
	// Destructor
	~ErrorShader();
};

