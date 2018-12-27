#include "GLProgram.h"

#define SCREEN_WIDTH	1600
#define SCREEN_HEIGHT	900


GLCinematicProgram::GLCinematicProgram()
	: GLProgram(RenderMode::CINEMATIC) 
{
	if (Camera::GetInstance() == NULL) {
		printf("Camera has not been setup");
		mError = true;
	}
}

void GLCinematicProgram::Run()
{
	if (mError) {
		return;
	}

	mRoot->SetUp();

	// Loop until window closed
	while (!mWindow->GetShouldClose()) {
		Time::Update();
		Input::NewFrame();

		// Get + Handle user input events
		glfwPollEvents();

		mRoot->Update();

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mRenderer->Render(R_ALL);

		glUseProgram(0);

		mWindow->SwapBuffers();
	}

	delete ErrorShader::GetInstance();
	delete mRenderer;
	delete mRoot;
	delete mWindow;
}


GLRoamProgram::GLRoamProgram()
	: GLProgram(RenderMode::ROAM) 
{
	if (Camera::GetInstance() == NULL) {
		printf("Camera has not been setup");
		mError = true;
	}
}

void GLRoamProgram::Run()
{
	if (mError) {
		return;
	}

	mRoot->SetUp();

	// Loop until window closed
	while (!mWindow->GetShouldClose()) {
		Time::Update();
		Input::NewFrame();

		// Get + Handle user input events
		glfwPollEvents();

		mRoot->Update();

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mRenderer->Render(R_ALL);

		glUseProgram(0);

		mWindow->SwapBuffers();
	}

	delete ErrorShader::GetInstance();
	delete mRenderer;
	delete mRoot;
	delete mWindow;
}

GLBakeProgram::GLBakeProgram()
	: GLProgram(RenderMode::BAKING) {}

void GLBakeProgram::Run()
{
	if (mError) {
		return;
	}

	printf("TODO: Bake the static shadow maps");
}


GLProgram::GLProgram(RenderMode mode) :
	mRenderMode(mode)
{
	mWindow = new GLWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	mWindow->Initialize();

	mRoot = new Transform();
	mRenderer = new GLRenderer();
	SceneLoader::Load("", mRenderer, mRoot, mWindow);
}

GLProgram::~GLProgram() {}

GLProgram* GLProgram::CreateGLProgramInstance(RenderMode mode)
{
	switch (mode) {
	case CINEMATIC:
		return new GLCinematicProgram();
	case ROAM:
		return new GLRoamProgram();
	case BAKING:
		return new GLBakeProgram();
	}
	return nullptr;
}
