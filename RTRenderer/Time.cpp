#include "Time.h"

GLfloat Time::mDTime = 0;
GLfloat Time::mTime = 0;
int Time::mFPS = 0;
int Time::mSecondsCounter = 0;

GLfloat Time::GetDeltaTime() {
	return mDTime;
}

void Time::Update() {
	GLfloat	cTime = glfwGetTime();
	mDTime = cTime - mTime;
	mTime = cTime;

	mFPS++;
	if (mTime - (GLfloat)mSecondsCounter >= 1.0) {
		printf("FPS: %d\n", mFPS);
		mFPS = 0;
		mSecondsCounter++;
	}
}