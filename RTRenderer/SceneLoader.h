#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include "GLRenderer.h"
#include "Transform.h"
#include "Model.h"

class SceneLoader
{
public:
	static void Load(const char* filename, GLRenderer* meshRenderer, Transform* transform);
	static void Store(const char* filename, GLRenderer* meshRenderer, Transform* transform);
};