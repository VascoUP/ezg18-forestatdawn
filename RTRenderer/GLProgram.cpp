#include "GLProgram.h"

#define SCREEN_WIDTH	1920
#define SCREEN_HEIGHT	1080

GLProgram* GLProgram::mInstance = nullptr;

GLCinematicProgram::GLCinematicProgram()
	: GLProgram(RenderMode::CINEMATIC) 
{
	SceneLoader::Load("", mRenderer, mRoot, mWindow, true);

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

	printf("Triangles:%d", VerticesCounter::GetNumberTriangles());

	mRoot->SetUp();

	mRenderer->BakeStage(mWindow);

	Time::Start();

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
	delete mWindow;
}


GLRoamProgram::GLRoamProgram()
	: GLProgram(RenderMode::ROAM) 
{
	SceneLoader::Load("", mRenderer, mRoot, mWindow, false);

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

	printf("Triangles:%d", VerticesCounter::GetNumberTriangles());

	bool updateObjects = true;

	mRoot->SetUp();

	mRenderer->BakeStage(mWindow);

	Time::Start();

	// Loop until window closed
	while (!mWindow->GetShouldClose()) {
		Time::Update();
		Input::NewFrame();

		// Get + Handle user input events
		glfwPollEvents();

		{
			bool isPressed = Input::IsKeyPress(GLFW_KEY_P);
			if (!mPauseWasPressed && isPressed) {
				mPauseWasPressed = true;
				updateObjects = !updateObjects;
			}
			else if (mPauseWasPressed && !isPressed) {
				mPauseWasPressed = false;
			}
		}

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
	if (mInstance != nullptr) {
		printf("Illegal operation");
		return;
	}

	mInstance = this;

	mWindow = new GLWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	mWindow->Initialize(false);
	
	mRoot = new Transform();
	mRenderer = new GLRenderer(mRoot);
}

GLProgram::~GLProgram() {}

GLProgram* GLProgram::GetInstance() {
	return mInstance;
}
 
GLRenderer* GLProgram::GetGLRenderer() {
	return mInstance->mRenderer;
}

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
