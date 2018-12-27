#pragma once

#include <stdio.h>
#include <algorithm>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

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

	int Initialize();
	void SetViewport();

	GLfloat GetBufferWidht();
	GLfloat GetBufferHeight();
	static int GetWidth() { return width; };
	static int GetHeight() { return height; };

	bool GetShouldClose();

	void SwapBuffers();

	~GLWindow();
};

