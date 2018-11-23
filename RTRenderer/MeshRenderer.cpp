#include "GLRenderer.h"

MeshRenderer::MeshRenderer(Transform *transform, int modelIndex)
{
	m_transform = transform;
	m_modelIndex = modelIndex;
}

bool MeshRenderer::FilterPass(RenderFilter filter)
{
	return (filter == R_ALL || 
		(filter == R_NSTATIC && !m_transform->GetStatic()) ||
		(filter == R_STATIC && m_transform->GetStatic()));
}

int MeshRenderer::GetModelIndex() const
{
	return m_modelIndex;
}

glm::mat4 MeshRenderer::GetTransformMatrix() const
{
	return m_transform->TransformMatrix(true);
}

MeshRenderer::~MeshRenderer()
{}
