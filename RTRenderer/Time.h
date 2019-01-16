#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Time {
private:
	static GLfloat mDTime;
	static GLfloat mTime;

	static int mFPS;
	static int mSecondsCounter;

public:
	static void Start();
	static GLfloat GetDeltaTime();
	static void Update();
};