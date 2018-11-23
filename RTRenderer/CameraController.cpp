#include "CameraController.h"

CameraController::CameraController(Transform* container, Camera* camera, GLfloat moveSpeed, GLfloat turnSpeed)
	: AObjectBehavior(container)
{
	this->camera = camera;
	this->moveSpeed = moveSpeed;
	this->turnSpeed = turnSpeed;
}

void CameraController::SetUp() {
	// Make sure that this scene object has a Camera
}

void CameraController::KeyControl() {
	GLfloat velocity = moveSpeed * Time::GetDeltaTime();
	if (Input::IsKeyPress(GLFW_KEY_W)) {
		this->transform->Translate(glm::vec3(0.0f, 0.0f, velocity));
	}
	if (Input::IsKeyPress(GLFW_KEY_S)) {
		this->transform->Translate(glm::vec3(0.0f, 0.0f, -velocity));
	}
	if (Input::IsKeyPress(GLFW_KEY_D)) {
		this->transform->Translate(glm::vec3(-velocity, 0.0f, 0.0f));
	}
	if (Input::IsKeyPress(GLFW_KEY_A)) {
		this->transform->Translate(glm::vec3(velocity, 0.0f, 0.0f));
	}
}

void CameraController::MouseControl() {
	GLfloat xDelta, yDelta;
	Input::GetMouseDelta(&xDelta, &yDelta);

	xDelta *= turnSpeed * Time::GetDeltaTime();
	yDelta *= turnSpeed * Time::GetDeltaTime();

	this->transform->Rotate(-yDelta, -xDelta, 0.0f);
}

void CameraController::Update() {
	KeyControl();
	MouseControl();
}

CameraController::~CameraController()
{
}
