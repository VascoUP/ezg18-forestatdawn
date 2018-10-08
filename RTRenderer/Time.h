#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Time {
private:
	static GLfloat deltaTime;
	static GLfloat lastTime;

public:
	static GLfloat GetDeltaTime();
	static void Update();
};