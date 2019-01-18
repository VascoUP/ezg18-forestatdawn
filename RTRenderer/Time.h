#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Time {
private:
	static double mDTime;
	static double mTime;

	static int mFPS;
	static int mSecondsCounter;

public:
	static void Start();
	static double GetDeltaTime();
	static void Update();
};