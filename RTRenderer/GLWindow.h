#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class GLWindow
{
private:
	GLFWwindow * mainWindow;

	GLint width;
	GLint height;
	GLint bufferWidth;
	GLint bufferHeight;

public:
	GLWindow();
	GLWindow(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLfloat GetBufferWidht();
	GLfloat GetBufferHeight();

	bool GetShouldClose();

	void SwapBuffers();

	~GLWindow();
};

