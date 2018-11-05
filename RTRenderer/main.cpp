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

#include "GLWindow.h"
#include "Time.h"
#include "Camera.h"
#include "CameraController.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Model.h"
#include "RotationObjects.h"

const float toRadians = 3.14159265359f / 180.0f;

GLWindow* glWindow;
Transform* rootObject;

MeshRenderer* meshRenderer;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader code
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CalcAvgNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLenght, unsigned int normalOffset) {
	for (size_t i = 0; i < indiceCount; i+=3) {
		unsigned int in0 = indices[i] * vLenght;
		unsigned int in1 = indices[i + 1] * vLenght;
		unsigned int in2 = indices[i + 2] * vLenght;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 n = glm::normalize(glm::cross(v1, v2));

		//glm::vec3 v3(vertices[in2] - vertices[in1], vertices[in2 + 1] - vertices[in1 + 1], vertices[in2 + 2] - vertices[in1 + 2]);

		in0 += normalOffset; 
		in1 += normalOffset; 
		in2 += normalOffset;

		vertices[in0] += n.x;
		vertices[in0 + 1] += n.y;
		vertices[in0 + 2] += n.z;
		vertices[in1] += n.x;
		vertices[in1 + 1] += n.y;
		vertices[in1 + 2] += n.z;
		vertices[in2] += n.x;
		vertices[in2 + 1] += n.y;
		vertices[in2 + 2] += n.z;
	}

	for (size_t i = 0; i < verticeCount / vLenght; i++) {
		unsigned int nOffset = i * vLenght + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects(Transform* parent) {
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	GLfloat vertices[] = {
		-1.0f, -1.0f, -0.6f,	0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		1.0f, 0.0f, 0.0f
	};

	CalcAvgNormals(indices, 12, vertices, 32, 8, 5);
	
	MeshInfo *info = new MeshInfo(); info->vertices = vertices; info->indices = indices; info->numOfVertices = 32; info->numOfIndices = 12;

	Transform* meshChild1 = new Transform(parent); 
	meshChild1->Translate(glm::vec3(0.0f, 0.0f, 5.0f)); 
	Mesh *tri1 = new Mesh(meshChild1, info);
	tri1->SetTexture(meshRenderer->GetTextures()[0]); 
	tri1->Load(); 
	meshRenderer->AddModels(tri1);
	
	info = new MeshInfo(); info->vertices = vertices; info->indices = indices; info->numOfVertices = 32; info->numOfIndices = 12;

	Transform* meshChild2 = new Transform(parent);
	meshChild2->Translate(glm::vec3(0.0f, 2.0f, 5.0f));
	Mesh *tri2 = new Mesh(meshChild2, info);
	tri2->SetTexture(meshRenderer->GetTextures()[1]);
	tri2->Load();
	meshRenderer->AddModels(tri2);

	unsigned int quadIndices[] = {
		1, 2, 0,
		3, 2, 1,

		4, 6, 5,
		5, 6, 7,

		9, 10, 8, 
		11, 10, 9,

		12, 14, 13,
		13, 14, 15,

		17, 18, 16,
		19, 18, 17,

		20, 22, 21,
		21, 22, 23
	};

	GLfloat quadVertices[] = {
		// Side Front
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,

		// Side Back
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, -1.0f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,

		// Side Right
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,

		// Side Left
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,	1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		// Side Up
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, -1.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,

		// Side Down
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f
	};

	info = new MeshInfo(); info->vertices = quadVertices; info->indices = quadIndices; info->numOfVertices = 192; info->numOfIndices = 36;

	Transform* meshChild3 = new Transform(parent);
	meshChild3->Translate(glm::vec3(0.0f, -2.0f, 5.0f));
	meshChild3->AddUpdatable(new RotatingObject(meshChild3, 0.0f, 1.0f, 0.0f));
	Mesh *quad1 = new Mesh(meshChild3, info);
	quad1->SetTexture(meshRenderer->GetTextures()[1]);
	quad1->Load();
	meshRenderer->AddModels(quad1);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	meshRenderer->AddShader(shader1);
}

int main() {
	glWindow = new GLWindow(1600, 900);
	glWindow->Initialize();

	rootObject = new Transform();
	rootObject->Translate(glm::vec3(0.0f, 0.0f, 0.0f));

	/*
	---------------------------------
		START SECTION TO BE MOVED
	---------------------------------
	*/
	meshRenderer = new MeshRenderer();
	meshRenderer->SetLight(new Light(1.0f, 1.0f, 1.0f, 0.1f, -1.0f, -1.0f, 2.0f, 0.6f));
	meshRenderer->AddTexture("Textures/brick.png");
	meshRenderer->AddTexture("Textures/dirt.png");
	
	CreateShaders();
	CreateObjects(rootObject);
	
	Transform object(rootObject);
	object.AddUpdatable(Camera::CreateInstance(&object, glWindow));
	object.AddUpdatable(new CameraController(&object, Camera::GetInstance(), 5.0f, 5.0f));
	
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

		// Get + Handle user input events
		glfwPollEvents();

		rootObject->Update();

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		meshRenderer->Render();

		glUseProgram(0);

		glWindow->SwapBuffers();
	}

	delete meshRenderer;
	delete ErrorShader::GetInstance();
	delete glWindow;

	return 0;
}