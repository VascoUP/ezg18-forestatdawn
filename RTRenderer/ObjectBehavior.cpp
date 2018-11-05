#include "ObjectBehavior.h"

ObjectBehavior::ObjectBehavior(Transform *container)
{
	this-> transform = container;
}

Transform * ObjectBehavior::GetTransform()
{
	return transform;
}

ObjectBehavior::~ObjectBehavior()
{
}
