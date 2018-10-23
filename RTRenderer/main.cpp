#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "GLWindow.h"
#include "Time.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "CameraController.h"
#include "RotationObjects.h"
#include "ObjectController.h"

const float toRadians = 3.14159265359f / 180.0f;

GLWindow *glWindow;
Transform* rootObject;
std::vector<Shader> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint shader, uniformModel, uniformProjection;

// Vertex Shader code
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects(Transform* parent) {
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Transform* meshChild1 = new Transform(parent);
	meshChild1->Translate(glm::vec3(0.0f, 0.0f, 5.0f));
	Mesh *tri1 = new Mesh(meshChild1);
	tri1->CreateMesh(&shaderList[0], vertices, indices, 12, 12);
	meshChild1->AddRenderable(tri1);
	meshChild1->AddUpdatable(new ObjectController(meshChild1, 2.0f, 1.5f));
	
	Transform* meshChild2 = new Transform(meshChild1);
	meshChild2->Translate(glm::vec3(0.0f, 1.0f, 0.0f));
	meshChild2->Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	Mesh *tri2 = new Mesh(meshChild2);
	tri2->CreateMesh(&shaderList[0], vertices, indices, 12, 12);
	meshChild2->AddRenderable(tri2);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {
	glWindow = new GLWindow(800, 600);
	glWindow->Initialize();

	rootObject = new Transform();
	rootObject->Translate(glm::vec3(0.0f, 0.0f, 0.0f));

	CreateShaders();
	CreateObjects(rootObject);
	
	Transform object(rootObject);
	object.AddUpdatable(Camera::CreateInstance(&object, glWindow));
	object.AddUpdatable(new CameraController(&object, Camera::GetInstance(), 5.0f, 5.0f));
	
	if (Camera::GetInstance() == NULL) {
		printf("Camera has not been setup");
		return 1;
	}

	rootObject->SetUp();

	// Loop until window closed
	while (!glWindow->GetShouldClose()) {
		Time::Update();

		// Get + Handle user input events
		glfwPollEvents();

		rootObject->Update();

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rootObject->Render();

		glUseProgram(0);

		glWindow->SwapBuffers();
	}

	free(ErrorShader::GetInstance());
	free(glWindow);

	return 0;
}