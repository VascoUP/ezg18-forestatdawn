#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <GL\glew.h>

class ShadowMap
{
protected:
	GLuint mFBO;
	GLuint mSM;
	GLuint mSWidth;
	GLuint mSHeight;

public:
	ShadowMap();

	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);

	GLuint GetShadowWidth();
	GLuint GetShadowHeight();

	~ShadowMap();
};

