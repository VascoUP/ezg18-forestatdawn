#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <assimp\Importer.hpp>

#include "Commons.h"

#include "GLWindow.h"
#include "GLRenderer.h"
#include "Time.h"
#include "SceneLoader.h"
#include "Model.h"
#include "Mesh.h"
#include "CameraController.h"
#include "RotationObjects.h"
#include "ObjectController.h"

GLWindow* glWindow;
GLRenderer* meshRenderer;
Transform* rootObject;

int main() {
	glWindow = new GLWindow(1600, 900);
	glWindow->Initialize();

	rootObject = new Transform();
	meshRenderer = new GLRenderer();
	SceneLoader::Load("", meshRenderer, rootObject);

	/*
	---------------------------------
		START SECTION TO BE MOVED
	---------------------------------
	*/
	Transform *xwingTransform = new Transform(rootObject);
	xwingTransform->Translate(glm::vec3(-15.0f, 0.0f, 15.0f));
	xwingTransform->Scale(0.01f);
	meshRenderer->AddMeshRenderer(new MeshRenderer(xwingTransform, 0));

	Transform *blackhawkTransform = new Transform(rootObject);
	blackhawkTransform->Scale(0.5f);
	blackhawkTransform->Translate(glm::vec3(5.0f, 5.0f, 0.0f));
	blackhawkTransform->Rotate(-1.57f, 0.0f, 0.0f);
	meshRenderer->AddMeshRenderer(new MeshRenderer(blackhawkTransform, 1));

	Transform *tree01Transform = new Transform(rootObject);
	tree01Transform->Translate(glm::vec3(-5.0f, 0.0f, 0.0f));
	meshRenderer->AddMeshRenderer(new MeshRenderer(tree01Transform, 2));
	Transform *tree01Transform2 = new Transform(rootObject);
	tree01Transform2->Translate(glm::vec3(-5.0f, 0.0f, -5.0f));
	meshRenderer->AddMeshRenderer(new MeshRenderer(tree01Transform2, 2));

	Transform *tree02Transform = new Transform(rootObject);
	meshRenderer->AddMeshRenderer(new MeshRenderer(tree02Transform, 3));
	
	// Camera object is not static
	Transform *object = new Transform(rootObject);
	object->SetStatic(false);
	object->Translate(glm::vec3(0.0f, 5.0f, -5.0f));
	object->AddUpdatable(Camera::CreateInstance(object, glWindow));
	object->AddUpdatable(new CameraController(object, Camera::GetInstance(), 5.0f, 5.0f));
	
	if (Camera::GetInstance() == NULL) {
		printf("Camera has not been setup");
		return 1;
	}
	/*
	---------------------------------
		END SECTION TO BE MOVED
	---------------------------------
	*/

	rootObject->SetUp();

	// Loop until window closed
	while (!glWindow->GetShouldClose()) {
		Time::Update();
		Input::NewFrame();

		// Get + Handle user input events
		glfwPollEvents();

		rootObject->Update();

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		meshRenderer->Render(R_ALL);

		glUseProgram(0);

		glWindow->SwapBuffers();
	}

	delete ErrorShader::GetInstance();
	delete meshRenderer;
	delete rootObject;
	delete glWindow;

	return 0;
}