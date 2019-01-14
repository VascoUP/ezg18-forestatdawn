#include "Camera.h"

constexpr float CAMERA_ANGLE = glm::radians(45.0f);

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
	projectionMatrix = glm::perspective(CAMERA_ANGLE, (GLfloat)window->GetBufferWidht() / (GLfloat)window->GetBufferHeight(), 0.1f, 100.0f);
}

void Camera::SetUp() {}

void Camera::Update() {}

glm::mat4 Camera::GetViewMatrix() {
	glm::vec3 position = transform->GetPosition();
	glm::vec4 wPosition = transform->parent->GetWorldMatrix() * glm::vec4(position.x, position.y, position.z, 1.0f);
	position = glm::vec3(wPosition.x, wPosition.y, wPosition.z);
	return glm::lookAt(position, position + transform->GetFront(), transform->GetUp());
}

glm::mat4 Camera::GetProjectionMatrix() {
	return projectionMatrix;
}

bool Camera::PointInsideViewFrustum(float cameraAngle, glm::vec3* cameraPosition, glm::vec3* cameraFront, glm::vec3* point, float bias) {
	glm::vec3 nPoint = glm::normalize(*point - *cameraPosition);
	float dotProd = glm::dot(*cameraFront, nPoint);
	float cosAngle = glm::cos(cameraAngle + bias);
	return dotProd >= cosAngle;
}

bool Camera::PointInsideViewFrustum(glm::vec3* point, float bias) {
	return Camera::PointInsideViewFrustum(CAMERA_ANGLE, &transform->GetPosition(), &transform->GetFront(), point, bias);
}

Camera::~Camera() {
}
