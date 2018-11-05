#include "CameraController.h"

CameraController::CameraController(Transform* container, Camera* camera, GLfloat moveSpeed, GLfloat turnSpeed)
	: ObjectBehavior(container)
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

	xDelta *= turnSpeed;
	yDelta *= turnSpeed;

	this->transform->Rotate(glm::vec3(yDelta, -xDelta, 0.0f) * Time::GetDeltaTime());
}

void CameraController::Update() {
	KeyControl();
	MouseControl();
}

CameraController::~CameraController()
{
}
