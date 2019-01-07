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

	// TODO: Calculate static shadow maps
	mRenderer->BakeShadowMaps(mWindow);

	// Loop until window closed
	while (!mWindow->GetShouldClose()) {
		Time::Update();
		Input::NewFrame();
		// Get + Handle user input events
		glfwPollEvents();
		// Update all objects
		mRoot->Update();
		// Render scene
		mRenderer->Render(mWindow, mRoot, R_ALL);
		// Set shader to be default
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

	bool updateObjects = true;

	mRoot->SetUp();

	// TODO: Calculate static shadow maps
	mRenderer->BakeShadowMaps(mWindow);

	// Loop until window closed
	while (!mWindow->GetShouldClose()) {
		Time::Update();
		Input::NewFrame();

		// Get + Handle user input events
		glfwPollEvents();

		if (Input::IsKeyPress(GLFW_KEY_P))
			updateObjects = !updateObjects;

		if (updateObjects)
			mRoot->Update();
		else
			Camera::GetInstance()->GetTransform()->Update();

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mRenderer->Render(mWindow, mRoot, R_ALL);

		glUseProgram(0);

		mWindow->SwapBuffers();
	}

	delete ErrorShader::GetInstance();
	delete mRenderer;
	delete mRoot;
	delete mWindow;
}


GLProgram::GLProgram(RenderMode mode) :
	mRenderMode(mode)
{
	mWindow = new GLWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	mWindow->Initialize();

	mRoot = new Transform();
	mRenderer = new GLRenderer();
	SceneLoader::Load("", mRenderer, mRoot, mWindow);

	mRenderer->Initialize(mRoot);
}

GLProgram::~GLProgram() {}

GLProgram* GLProgram::CreateGLProgramInstance(RenderMode mode)
{
	switch (mode) {
	case CINEMATIC:
		return new GLCinematicProgram();
	case ROAM:
		return new GLRoamProgram();
	}
	return nullptr;
}
