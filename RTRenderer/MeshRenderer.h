#pragma once

#include "Transform.h"

class MeshRenderer 
{
private:
	Transform* m_transform;
	int m_modelIndex;
public:
	MeshRenderer(Transform *transform, int modelIndex);

	bool FilterPass(RenderFilter filter);
	int GetModelIndex() const;
	glm::mat4 GetTransformMatrix() const;

	~MeshRenderer();
};

