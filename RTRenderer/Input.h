#pragma once

#include <stdio.h>
#include <algorithm>

#include <GLFW\glfw3.h>

#include "Commons.h"

class Input
{
public:
	static void Initialize(GLFWwindow *window);

	static bool IsKeyPress(int key);

	static void GetMousePosition(GLfloat *xPosition, GLfloat *yPosition);
	static void GetMouseDelta(GLfloat *xDelta, GLfloat *yDelta);
	
	static void NewFrame();

private:
	static bool m_keys[];

	static GLfloat m_mouseVirtualX;
	static GLfloat m_mouseVirtualY;
	static GLfloat m_mouseX;
	static GLfloat m_mouseY;
	static GLfloat m_deltaX;
	static GLfloat m_deltaY;
	static bool m_mouseFirstMove;

	static void _CreateCallbacks(GLFWwindow *window);
	// This is a callback function for the GLFW to call whenever a key is pressed
	static void _HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	// This is a callback function for the GLFW to call the mouse moves
	static void _HandleMouse(GLFWwindow *window, double xPosition, double yPosition);
};

