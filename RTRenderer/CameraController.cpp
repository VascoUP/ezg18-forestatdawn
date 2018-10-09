#include "CameraController.h"

CameraController::CameraController(const SceneObject* container, Camera* camera, GLfloat moveSpeed, GLfloat turnSpeed)
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
		camera->position += camera->front * velocity;
	}
	if (Input::IsKeyPress(GLFW_KEY_S)) {
		camera->position -= camera->front * velocity;
	}
	if (Input::IsKeyPress(GLFW_KEY_D)) {
		camera->position += camera->right * velocity;
	}
	if (Input::IsKeyPress(GLFW_KEY_A)) {
		camera->position -= camera->right * velocity;
	}
}

void CameraController::MouseControl() {
	GLfloat xDelta, yDelta;
	Input::GetMouseDelta(&xDelta, &yDelta);

	xDelta *= turnSpeed;
	yDelta *= turnSpeed;

	camera->yaw += xDelta;
	camera->pitch += yDelta;

	if (camera->pitch > 89.0f)
	{
		camera->pitch = 89.0f;
	}
	else if (camera->pitch < -89.0f)
	{
		camera->pitch = -89.0f;
	}
}

void CameraController::Update() {
	KeyControl();
	MouseControl();
}

CameraController::~CameraController()
{
}
