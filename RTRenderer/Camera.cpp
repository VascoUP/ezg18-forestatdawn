#include "Camera.h"

/* 
---------------------------
 Enforce Singleton pattern
---------------------------
*/
Camera* Camera::camera = 0;

Camera* Camera::CreateInstance(Transform* object, GLWindow* window) {
	camera = new Camera(object, window);
	return camera;
}

Camera* Camera::GetInstance() {
	return camera;
}

glm::vec3 Camera::GetCameraPosition()
{
	return transform->GetPosition();
}

Camera::Camera(Transform* object, GLWindow* window) :
	AObjectBehavior(object)
{
	projectionMatrix = glm::perspective(45.0f, (GLfloat)window->GetBufferWidht() / (GLfloat)window->GetBufferHeight(), 0.1f, 100.0f);
}

void Camera::SetUp() {}

void Camera::Update() {}

glm::mat4 Camera::CalculateViewMatrix() {
	glm::vec3 position = transform->GetPosition();
	glm::vec4 wPosition = transform->parent->GetWorldMatrix() * glm::vec4(position.x, position.y, position.z, 1.0f);
	position = glm::vec3(wPosition.x, wPosition.y, wPosition.z);
	return glm::lookAt(position, position + transform->GetFront(), transform->GetUp());
}

glm::mat4 Camera::ProjectionMatrix() {
	return projectionMatrix;
}

Camera::~Camera() {
}
