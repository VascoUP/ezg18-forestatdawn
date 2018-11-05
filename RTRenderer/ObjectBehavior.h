#pragma once

#include "Transform.h"
#include "Updatable.h"
#include "SceneObject.h"

class ObjectBehavior : public Updatable
{
protected:
	Transform* transform;
public:
	ObjectBehavior(Transform* container);

	Transform* GetTransform();

	~ObjectBehavior();
};

