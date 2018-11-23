#include "Input.h"

bool Input::m_keys[1024];
GLfloat Input::m_mouseVirtualX = 0;
GLfloat Input::m_mouseVirtualY = 0;
GLfloat Input::m_mouseX = 0;
GLfloat Input::m_mouseY = 0;
GLfloat Input::m_deltaX = 0;
GLfloat Input::m_deltaY = 0;
bool Input::m_mouseFirstMove = true;

void Input::Initialize(GLFWwindow *window) {
	std::fill_n(m_keys, 1024, false);

	_CreateCallbacks(window);
}

bool Input::IsKeyPress(int key) {
	if (key < 0 || key >= 1024) {
		return false;
	}
	return m_keys[key];
}

void Input::GetMousePosition(GLfloat *xPosition, GLfloat *yPosition) {
	*xPosition = m_mouseVirtualX;
	*yPosition = m_mouseVirtualY;
}

void Input::GetMouseDelta(GLfloat *xDelta, GLfloat *yDelta) {
	*xDelta = m_deltaX;
	*yDelta = m_deltaY;
}

void Input::_CreateCallbacks(GLFWwindow *window) {
	glfwSetKeyCallback(window, _HandleKeys);
	glfwSetCursorPosCallback(window, _HandleMouse);
}

void Input::_HandleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			m_keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			m_keys[key] = false;
		}
	}
}

void Input::_HandleMouse(GLFWwindow *window, double xPosition, double yPosition) {
	if (m_mouseFirstMove) {
		m_mouseVirtualX = (GLfloat)xPosition;
		m_mouseVirtualY = (GLfloat)yPosition;
		m_mouseX = (GLfloat)xPosition;
		m_mouseY = (GLfloat)yPosition;
		m_mouseFirstMove = false;
		return;
	}

	GLfloat deltaX = (GLfloat)xPosition - m_mouseX;
	GLfloat deltaY = m_mouseY - (GLfloat)yPosition;

	m_deltaX = Commons::LSmoothStep(m_deltaX, deltaX, 0.2f);
	m_deltaY = Commons::LSmoothStep(m_deltaY, deltaY, 0.2f);

	m_mouseX = (GLfloat)xPosition;
	m_mouseY = (GLfloat)yPosition;
}

void Input::NewFrame()
{
	m_deltaX *= 0.8f;
	m_deltaY *= 0.8f;

	m_mouseVirtualX += m_deltaX;
	m_mouseVirtualY += m_deltaY;
}
