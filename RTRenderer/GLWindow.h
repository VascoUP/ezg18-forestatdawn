#pragma once

#include <stdio.h>
#include <algorithm>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <GLFW\glfw3native.h>

#include "Input.h"

class GLWindow
{
private:
	GLFWwindow * mainWindow;

	static GLint width;
	static GLint height;
	GLint bufferWidth;
	GLint bufferHeight;
public:
	GLWindow(GLint windowWidth, GLint windowHeight);

	bool Initialize(bool fullScreen);
	void SetViewport();

	GLuint GetBufferWidht();
	GLuint GetBufferHeight();
	static int GetWidth() { return width; };
	static int GetHeight() { return height; };

	bool GetShouldClose();

	void SwapBuffers();

	~GLWindow();
};

