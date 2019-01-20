#pragma once

#include "RenderMode.h"
#include "GLWindow.h"
#include "Transform.h"
#include "GLRenderer.h"
#include "SceneLoader.h"
#include "ObjectController.h"

class GLProgram
{
protected:
	static GLProgram* mInstance;
	
	const RenderMode mRenderMode;
	
	GLWindow* mWindow;
	Transform* mRoot;
	GLRenderer* mRenderer;

	bool mError = false;

	GLProgram(RenderMode mode);
public:
	static GLProgram* CreateGLProgramInstance(RenderMode mode);

	static GLProgram* GetInstance();
	static GLRenderer* GetGLRenderer();

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
private:
	friend class GLProgram;

	bool mPauseWasPressed = false;

	GLRoamProgram();
public:
	void Run();
};