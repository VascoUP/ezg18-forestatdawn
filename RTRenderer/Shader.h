#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "ShaderCompiler.h"
#include "ErrorShader.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Commons.h"

// This class holds the information needed to run simple shader program
class Shader
{
private:
	// ID of the shader
	GLuint shaderID;
public:
	// Constructor
	Shader();

	// Given the vertex and fragment shader code it creates a shader program
	bool CreateFromString(const char* vertexCode, const char* fragmentCode);
	// Given the vertex and fragment shader files it creates a shader program
	bool CreateFromFiles(const char* vertexFile, const char* fragmentFile);

	// Activates this shader whenever it's called
	void UseShader();
	// Frees the memory allocated by the shader program
	void ClearShader();

private:
	// Reads shader code from a file
	std::string ReadFile(const char* file);

protected:
	GLuint GetUniformLocation(const char* uniform);

	virtual void GetShaderUniforms() = 0;

public:
	// Destructor
	~Shader();

};

