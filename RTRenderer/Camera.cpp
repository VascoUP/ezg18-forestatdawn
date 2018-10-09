#include "Camera.h"

/* 
---------------------------
 Enforce Singleton pattern
---------------------------
*/
Camera* Camera::camera = 0;

Camera* Camera::CreateInstance(const SceneObject* object, glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) {
	camera = new Camera(object, position, up, yaw, pitch);
	return camera;
}

Camera* Camera::GetInstance() {
	return camera;
}


Camera::Camera(const SceneObject* object, glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) :
	ObjectBehavior(object)
{
	this->position = position;
	this->_worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	
	Update();
}

void Camera::SetUp() {

}

glm::mat4 Camera::CalculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::Update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, _worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {
}
