#include "Input.h"

bool Input::keys[1024];
GLfloat Input::previousX = 0;
GLfloat Input::previousY = 0;
GLfloat Input::deltaX = 0;
GLfloat Input::deltaY = 0;
bool Input::mouseFirstMove = true;

void Input::Initialize(GLFWwindow *window) {
	std::fill_n(keys, 1024, false);

	CreateCallbacks(window);
}

bool Input::IsKeyPress(int key) {
	if (key < 0 || key >= 1024) {
		return false;
	}
	return keys[key];
}

void Input::GetMousePosition(GLfloat *xPosition, GLfloat *yPosition) {
	*xPosition = previousX;
	*yPosition = previousY;
}

void Input::GetMouseDelta(GLfloat *xDelta, GLfloat *yDelta) {
	*xDelta = deltaX;
	*yDelta = deltaY;
	// This is over simplified
	deltaX = 0;
	deltaY = 0;
}

void Input::CreateCallbacks(GLFWwindow *window) {
	glfwSetKeyCallback(window, HandleKeys);
	glfwSetCursorPosCallback(window, HandleMouse);
}

void Input::HandleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
}

void Input::HandleMouse(GLFWwindow *window, double xPosition, double yPosition) {
	if (mouseFirstMove) {
		previousX = xPosition;
		previousY = yPosition;
		mouseFirstMove = false;
		return;
	}

	deltaX = xPosition - previousX;
	deltaY = previousY - yPosition;

	previousX = xPosition;
	previousY = yPosition;

	printf("Delta X:%.6f delta Y:%.6f\n", deltaX, deltaY);
}
