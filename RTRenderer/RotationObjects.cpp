#include "RotationObjects.h"


RotatingObject::RotatingObject(Transform* container, float velX, float velY, float velZ)
	: ObjectBehavior(container)
{
	rotationVelocityX = velX;
	rotationVelocityY = velY;
	rotationVelocityZ = velZ;
}

void RotatingObject::Update() {
	transform->Rotate(glm::vec3(rotationVelocityX, rotationVelocityY, rotationVelocityZ) * Time::GetDeltaTime());
}

RotatingObject::~RotatingObject()
{
}
