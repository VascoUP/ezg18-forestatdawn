#include "Renderable.h"

Renderable::Renderable(Transform* container) {
	transform = container;
}

Transform * Renderable::GetTransform()
{
	return transform;
}

void Renderable::Load() {}

void Renderable::Render() {}

void Renderable::Clear() {}

Renderable::~Renderable()
{}
