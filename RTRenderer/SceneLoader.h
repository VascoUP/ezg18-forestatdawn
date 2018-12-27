#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include "GLRenderer.h"
#include "Transform.h"
#include "Model.h"
#include "GLWindow.h"
#include "TerrainMesh.h"
#include "CameraController.h"
#include "ObjectController.h"


class SceneLoader
{
public:
	static void Load(const char* filename, GLRenderer* meshRenderer, Transform* transform, GLWindow* glWindow);
	static void Store(const char* filename, GLRenderer* meshRenderer, Transform* transform);
};