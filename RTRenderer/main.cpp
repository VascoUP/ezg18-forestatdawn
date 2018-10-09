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
#include "SceneObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "CameraController.h"

const float toRadians = 3.14159265359f / 180.0f;

GLWindow *glWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint shader, uniformModel, uniformProjection;

// Vertex Shader code
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() {
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

	Mesh *tri1 = new Mesh();
	tri1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(tri1);

	Mesh *tri2 = new Mesh();
	tri2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(tri2);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {
	glWindow = new GLWindow(800, 600);
	glWindow->Initialize();

	CreateObjects();
	CreateShaders();

	SceneObject object(new Transform());
	object.AddUpdatable(Camera::CreateInstance(&object, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0));
	object.AddUpdatable(new CameraController(&object, Camera::GetInstance(), 5.0f, 1.0f));

	GLuint uniformProjection = 0, uniformView = 0, uniformModel = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)glWindow->GetBufferWidht() / (GLfloat)glWindow->GetBufferHeight(), 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	float rot = 0;
	float deltaRot = 0.002f;

	if (Camera::GetInstance() == NULL) {
		printf("Camera has not been setup");
		return 1;
	}

	// Loop until window closed
	while (!glWindow->GetShouldClose()) {
		Time::Update();

		// Get + Handle user input events
		glfwPollEvents();

		rot += deltaRot;

		object.Update();

		Camera::GetInstance()->Update();

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation(); 
		uniformView = shaderList[0].GetViewLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -2.5f));
		model = glm::rotate(model, rot, glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->CalculateViewMatrix()));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->Render();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.5f));
		model = glm::rotate(model, rot, glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->CalculateViewMatrix()));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[1]->Render();

		glUseProgram(0);

		glWindow->SwapBuffers();
	}

	free(ErrorShader::GetInstance());
	free(glWindow);

	return 0;
}