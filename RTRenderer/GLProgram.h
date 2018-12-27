#pragma once

#include "RenderMode.h"
#include "GLWindow.h"
#include "Transform.h"
#include "GLRenderer.h"
#include "SceneLoader.h"

class GLProgram
{
protected:
	const RenderMode mRenderMode;
	GLWindow* mWindow;
	Transform* mRoot;
	GLRenderer* mRenderer;

	bool mError = false;
	
	GLProgram(RenderMode mode);
public:
	static GLProgram* CreateGLProgramInstance(RenderMode mode);

	virtual void Run() = 0;

	~GLProgram();
};

class GLCinematicProgram : public GLProgram {
	friend class GLProgram;

	GLCinematicProgram();
public:
	void Run();
};

class GLRoamProgram : public GLProgram {
	friend class GLProgram;

	GLRoamProgram();
public:
	void Run();
};

class GLBakeProgram : public GLProgram {
	friend class GLProgram;

	GLBakeProgram();
public:
	void Run();
};