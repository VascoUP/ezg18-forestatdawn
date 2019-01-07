#pragma once

#include <stdio.h>

#include <GL\glew.h>

class CubeMap
{
private:
	GLuint mFBO;
	GLuint mCubeMap;
	GLuint mSWidth;
	GLuint mSHeight;

	GLfloat mFar;
	GLfloat mNear;
	GLfloat mAspect;

public:
	CubeMap();

	bool Init(GLuint width, GLuint height, GLfloat near, GLfloat far);
	void Write();
	void Read(GLenum textureUnit);

	GLuint GetFBO() { return mFBO; };
	GLuint GetCubeMap() { return mCubeMap; };
	GLuint GetShadowWidth();
	GLuint GetShadowHeight();
	GLfloat GetFar() { return mFar; };
	GLfloat GetNear() { return mNear; };
	GLfloat GetAspect() { return mAspect; };

	~CubeMap();
};