#include "SceneObject.h"

int SceneObject::NEXT_ID = 1;

SceneObject::SceneObject() {
	this->transform = new Transform(this);
}

SceneObject::SceneObject(Transform* parent)
{
	this->transform = new Transform(this, parent);
}

void SceneObject::SetUp() {
	for (std::vector<Updatable*>::iterator it = updatables.begin(); it != updatables.end(); it++)
	{
		(*it)->SetUp();
	}

	// Iterate over children
	for (std::vector<Transform*>::iterator it = transform->GetChildren().begin(); it != transform->GetChildren().end(); it++)
	{
		(*it)->GetContainer().SetUp();
	}
}

void SceneObject::Update() {
	// Skip Update if the object is not active
	if (!active)
		return;

	for (std::vector<Updatable*>::iterator it = updatables.begin(); it != updatables.end(); it++)
	{
		(*it)->Update();
	}

	// Iterate over children
	for (std::vector<Transform*>::iterator it = transform->GetChildren().begin(); it != transform->GetChildren().end(); it++)
	{
		(*it)->GetContainer().Update();
	}
}

void SceneObject::Render() {
	if (!active)
		return;

	for (std::vector<Renderable*>::iterator it = renderables.begin(); it != renderables.end(); it++)
	{
		(*it)->Render();
	}

	// Iterate over children
	for (std::vector<Transform*>::iterator it = transform->GetChildren().begin(); it != transform->GetChildren().end(); it++)
	{
		(*it)->GetContainer().Render();
	}
}

void SceneObject::SetActive(bool active) {
	this->active = active;
}

void SceneObject::AddUpdatable(Updatable* updatable) {
	updatables.push_back(updatable);
}

void SceneObject::AddRenderable(Renderable* renderable) {
	renderables.push_back(renderable);
}

Transform* SceneObject::GetTransform() {
	return transform;
}

SceneObject::~SceneObject()
{
	free(transform);
}
