#pragma once

#include "Updatable.h"
#include "SceneObject.h"

class ObjectBehavior : public Updatable
{
private:
	const SceneObject* container;
public:
	ObjectBehavior(const SceneObject* container);
	~ObjectBehavior();
};

