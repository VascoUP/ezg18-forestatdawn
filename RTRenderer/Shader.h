#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include "ShaderCompiler.h"
#include "ErrorShader.h"

// This class holds the information needed to run simple shader program
class Shader
{
private:
	// ID of the shader
	GLuint shaderID;
	// Projection matrix
	GLuint uniformProjection;
	// View matrix
	GLuint uniformView;
	// Model matrix
	GLuint uniformModel;

public:
	// Constructor
	Shader();

	// Given the vertex and fragment shader code it creates a shader program
	bool CreateFromString(const char* vertexCode, const char* fragmentCode);
	// Given the vertex and fragment shader files it creates a shader program
	bool CreateFromFiles(const char* vertexFile, const char* fragmentFile);
	// Reads shader code from a file
	std::string ReadFile(const char* file);
	// Getter for uniformProjection
	GLuint GetProjectionLocation();
	// Getter for uniformView
	GLuint GetViewLocation();
	// Getter for uniformModel
	GLuint GetModelLocation();

	// Activates this shader whenever it's called
	void UseShader();
	// Frees the memory allocated by the shader program
	void ClearShader();

	// Destructor
	~Shader();
};

