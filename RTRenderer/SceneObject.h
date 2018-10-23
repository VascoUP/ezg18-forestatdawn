#pragma once

#include <stdio.h>
#include <vector> 
#include <algorithm>

#include "Transform.h"
#include "Updatable.h"
#include "Renderable.h"

class SceneObject: public Updatable, Renderable
{
private:
	static int NEXT_ID;
	const int OBJECT_ID = NEXT_ID++;

	bool active = true;
	Transform* transform;
	std::vector<Updatable*> updatables;
	std::vector<Renderable*> renderables;

public:
	SceneObject();
	SceneObject(Transform* transform);

	void SetUp();
	void Update(); 
	void Render();

	void SetActive(bool active);

	Transform* GetTransform();

	void AddUpdatable(Updatable* updatable);
	void AddRenderable(Renderable* renderable);
	//void RemoveUpdatable(Updatable* updatable);

	~SceneObject();
};

