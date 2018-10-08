#include "Camera.h"

Camera* Camera::camera = 0;

Camera* Camera::CreateInstance(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed) {
	camera = new Camera(position, up, yaw, pitch, moveSpeed, turnSpeed);
	return camera;
}

Camera* Camera::GetInstance() {
	return camera;
}

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed) {
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->moveSpeed = moveSpeed;
	this->turnSpeed = turnSpeed;

	Update();
}

void Camera::SetUp() {

}

glm::mat4 Camera::CalculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::KeyControl() {
	GLfloat velocity = moveSpeed * Time::GetDeltaTime();
	if (Input::IsKeyPress(GLFW_KEY_W)) {
		position += front * velocity;
	}
	if (Input::IsKeyPress(GLFW_KEY_S)) {
		position -= front * velocity;
	}
	if (Input::IsKeyPress(GLFW_KEY_D)) {
		position += right * velocity;
	}
	if (Input::IsKeyPress(GLFW_KEY_A)) {
		position -= right * velocity;
	}
}

void Camera::MouseControl() {
	GLfloat xDelta, yDelta;
	Input::GetMouseDelta(&xDelta, &yDelta);

	xDelta *= turnSpeed;
	yDelta *= turnSpeed;

	yaw += xDelta;
	pitch += yDelta;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	Update();
}

void Camera::Update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {
}
