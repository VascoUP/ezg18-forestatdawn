#include "Time.h"

double Time::mDTime = 0;
double Time::mTime = 0;
int Time::mFPS = 0;
int Time::mSecondsCounter = 0;

void Time::Start()
{
	mTime = glfwGetTime();
}

double Time::GetDeltaTime() {
	return mDTime;
}

void Time::Update() {
	double cTime = glfwGetTime();
	mDTime = cTime - mTime;
	mTime = cTime;

	mFPS++;
	if (mTime - (GLfloat)mSecondsCounter >= 1.0) {
		printf("FPS: %d\n", mFPS);
		mFPS = 0;
		mSecondsCounter++;
	}
}