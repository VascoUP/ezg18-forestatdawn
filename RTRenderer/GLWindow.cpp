#include "GLWindow.h"

int GLWindow::width = 1600;
int GLWindow::height = 900;

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;
}

bool GLWindow::Initialize(bool fullscreen) {
	// Initialize GLFW
	if (!glfwInit()) {
		printf("GLFW initialization failed.");
		glfwTerminate();
		return false;
	}


	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwared Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLU_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	if (!mainWindow) {
		printf("GLFW window creation failed.");
		glfwTerminate();
		return false;
	}

	// Get Buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for glfw to use
	glfwMakeContextCurrent(mainWindow);

	// Set input interroption callbacks
	Input::Initialize(mainWindow);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return false;
	}

	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup viewport
	SetViewport();

	// Define the user of main window
	glfwSetWindowUserPointer(mainWindow, this);

	return true;
}

void GLWindow::SetViewport()
{
	glViewport(0, 0, bufferWidth, bufferHeight);
}

GLuint GLWindow::GetBufferWidht() {
	return bufferWidth;
}

GLuint GLWindow::GetBufferHeight() {
	return bufferHeight;
}

bool GLWindow::GetShouldClose() {
	return glfwWindowShouldClose(mainWindow);
}

void GLWindow::SwapBuffers() {
	glfwSwapBuffers(mainWindow);
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
