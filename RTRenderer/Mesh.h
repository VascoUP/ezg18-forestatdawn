#pragma once

#include <GL\glew.h>

#include "IRenderable.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

struct MeshInfo {
	GLfloat *vertices;
	unsigned int *indices;
	unsigned int numOfVertices;
	unsigned int numOfIndices;
};

//! This class holds the information required to draw a mesh onto a scene
class Mesh : public IRenderable 
{
private:
	//! Vertex array
	GLuint VAO;
	//! Vertex buffer
	GLuint VBO;
	//! Element buffer
	GLuint EBO;
	//! Number of indices
	GLsizei indexCount;
	//! Shader to be applied to this model
	Texture* texture;

	MeshInfo* meshInfo;
public:
	//! Constructor
	Mesh(MeshInfo* info);

	void SetTexture(Texture* tex);

	//! This function is responsible for creating a proper Mesh out of vertices and the respective index order
	void Load();
	//! This function renders the Mesh (if it exists) onto the scene
	void Render();
	//! This function frees the memory allocated by the Mesh
	void Clear();

	//! Destructor
	~Mesh();
};

