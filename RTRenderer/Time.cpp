#include "Time.h"

GLfloat Time::deltaTime = 0;
GLfloat Time::lastTime = 0;

GLfloat Time::GetDeltaTime() {
	return deltaTime;
}

void Time::Update() {
	GLfloat	currTime = glfwGetTime();
	deltaTime = currTime - lastTime;
	lastTime = currTime;
}