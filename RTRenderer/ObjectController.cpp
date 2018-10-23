#include "ObjectController.h"

ObjectController::ObjectController(Transform* container, float vel, float rotVel)
	: ObjectBehavior(container)
{
	moveSpeed = vel;
	rotSpeed = rotVel;
}

void ObjectController::SetUp() {

}

void ObjectController::Update() {
	GLfloat velocity = moveSpeed * Time::GetDeltaTime();
	if (Input::IsKeyPress(GLFW_KEY_W)) {
		this->transform->Translate(glm::vec3(0.0f, 0.0f, velocity));
	}
	if (Input::IsKeyPress(GLFW_KEY_S)) {
		this->transform->Translate(glm::vec3(0.0f, 0.0f, -velocity));
	}
	if (Input::IsKeyPress(GLFW_KEY_D)) {
		this->transform->Rotate(glm::vec3(0.0f, -velocity, 0.0f));
	}
	if (Input::IsKeyPress(GLFW_KEY_A)) {
		this->transform->Rotate(glm::vec3(0.0f, velocity, 0.0f));
	}
}

ObjectController::~ObjectController()
{
}
