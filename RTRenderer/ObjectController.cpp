#include "ObjectController.h"

ObjectController::ObjectController(Transform* container, float vel, float rotVel)
	: AObjectBehavior(container)
{
	moveSpeed = vel;
	rotSpeed = rotVel;
}

void ObjectController::SetUp() {

}

void ObjectController::Update() {
	GLfloat velocity = moveSpeed * Time::GetDeltaTime();
	GLfloat angularVelocity = rotSpeed * Time::GetDeltaTime();

	this->transform->Translate(glm::vec3(0.0f, velocity, 0.0f));
	this->transform->Rotate(0.0f, angularVelocity, 0.0f);
}

ObjectController::~ObjectController()
{
}
