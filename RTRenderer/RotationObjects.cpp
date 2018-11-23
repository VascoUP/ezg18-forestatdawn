#include "RotationObjects.h"


RotatingObject::RotatingObject(Transform* container, float velX, float velY, float velZ)
	: AObjectBehavior(container)
{
	rotationVelocityX = velX;
	rotationVelocityY = velY;
	rotationVelocityZ = velZ;
}

void RotatingObject::SetUp() {}

void RotatingObject::Update() {
	transform->Rotate(rotationVelocityX * Time::GetDeltaTime(), rotationVelocityY * Time::GetDeltaTime(), rotationVelocityZ * Time::GetDeltaTime());
}

RotatingObject::~RotatingObject()
{
}
