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
	ObjectBehavior(object)
{
	projectionMatrix = glm::perspective(45.0f, (GLfloat)window->GetBufferWidht() / (GLfloat)window->GetBufferHeight(), 0.1f, 100.0f);
}

void Camera::SetUp() {}

void Camera::Update() {}

glm::mat4 Camera::CalculateViewMatrix() {
	return glm::lookAt(transform->GetPosition(), transform->GetPosition() + transform->GetFront(), transform->GetUp());
}

glm::mat4 Camera::ProjectionMatrix() {
	return projectionMatrix;
}

Camera::~Camera() {
}
