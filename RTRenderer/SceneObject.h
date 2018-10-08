#pragma once

#include <stdio.h>
#include <vector> 
#include <algorithm>

#include "Updatable.h"
#include "Transform.h"

class SceneObject: public Updatable
{
private:
	Transform* transform;
	std::vector<Updatable*> updatables;

public:
	SceneObject(Transform* transform);

	void SetUp();
	void Update();

	Transform* GetTransform();

	void AddUpdatable(Updatable* updatable);
	void RemoveUpdatable(Updatable* updatable);

	~SceneObject();
};

