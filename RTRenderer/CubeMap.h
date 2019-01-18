#pragma once

#include <stdio.h>

#include <GL\glew.h>

class CubeMap
{
private:
	GLuint mFBO;
	GLuint mCubeMap;
	GLuint mDepthMap;
	GLuint mSWidth;
	GLuint mSHeight;

	GLfloat mFar;
	GLfloat mNear;
	GLfloat mAspect;

	bool mAdaptResolution = true;

public:
	CubeMap(GLfloat near, GLfloat far);

	bool Init(GLuint width, GLuint height, GLfloat near, GLfloat far);
	void Write();
	void Read(GLenum textureUnit);
	void Clear();

	void ReadyCubemap(float distToCamera);

	GLuint GetFBO() { return mFBO; };
	GLuint GetCubeMap() { return mCubeMap; };
	GLuint GetShadowWidth() { return mSWidth; };
	GLuint GetShadowHeight() { return mSHeight; };
	GLfloat GetFar() { return mFar; };
	GLfloat GetNear() { return mNear; };
	GLfloat GetAspect() { return mAspect; };

	~CubeMap();
};