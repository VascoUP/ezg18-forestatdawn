#pragma once

#include <stdio.h>
#include <algorithm>

#include <GLFW\glfw3.h>

class Input
{
public:
	static bool keys[];

	static GLfloat previousX;
	static GLfloat previousY;
	static GLfloat deltaX;
	static GLfloat deltaY;
	static bool mouseFirstMove;

	static void Initialize(GLFWwindow *window);

	static bool IsKeyPress(int key);

	static void GetMousePosition(GLfloat *xPosition, GLfloat *yPosition);
	static void GetMouseDelta(GLfloat *xDelta, GLfloat *yDelta);

private:
	static void CreateCallbacks(GLFWwindow *window);
	// This is a callback function for the GLFW to call whenever a key is pressed
	static void HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	// This is a callback function for the GLFW to call the mouse moves
	static void HandleMouse(GLFWwindow *window, double xPosition, double yPosition);
};

