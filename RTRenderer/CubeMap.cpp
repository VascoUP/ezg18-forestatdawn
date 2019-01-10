#include "CubeMap.h"

constexpr unsigned int SIZE1 = 512;
constexpr unsigned int SIZE2 = 256;
constexpr unsigned int SIZE3 = 128;
constexpr float DISTANCE1 = 2.0f;
constexpr float DISTANCE2 = 4.0f;

CubeMap::CubeMap(GLfloat near, GLfloat far)
{
	mNear = near; mFar = far;
}

bool CubeMap::Init(GLuint width, GLuint height, GLfloat near, GLfloat far)
{
	mSWidth = width; mSHeight = height;
	mNear = near; mFar = far;
	mAspect = (float)width / (float)height;

	glGenFramebuffers(1, &mFBO);

	glGenTextures(1, &mCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMap);

	for (size_t i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, mSWidth, mSHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mCubeMap, 0);
	
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %i\n", Status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void CubeMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
}

void CubeMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMap);
}

void CubeMap::ReadyCubemap(float distToCamera)
{
	if (mAdaptResolution) {
		if (distToCamera <= DISTANCE1) {
			if (mSWidth != SIZE1) {
				Clear();
				Init(SIZE1, SIZE1, mNear, mFar);
			}
		}
		else if (distToCamera <= DISTANCE2) {
			if (mSWidth != SIZE2) {
				Clear();
				Init(SIZE2, SIZE2, mNear, mFar);
			}
		}
		else if (mSWidth != SIZE3) {
			Clear();
			Init(SIZE3, SIZE3, mNear, mFar);
		}
	}
}

void CubeMap::Clear()
{
	if (mFBO)
		glDeleteFramebuffers(1, &mFBO);

	if (mCubeMap)
		glDeleteTextures(1, &mCubeMap);
}

CubeMap::~CubeMap()
{
	Clear();
}
