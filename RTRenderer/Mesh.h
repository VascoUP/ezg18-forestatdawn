#pragma once

#include <GL\glew.h>

#include "Updatable.h"
#include "Renderable.h"

// This class holds the information required to draw a mesh onto a scene
class Mesh: public Updatable, Renderable
{
private:
	// Vertex array
	GLuint VAO;
	// Vertex buffer
	GLuint VBO;
	// Element buffer
	GLuint EBO;
	// Number of indices
	GLsizei indexCount;

public:
	// Constructor
	Mesh();


	// This function is responsible for creating a proper Mesh out of vertices and the respective index order
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	// This function renders the Mesh (if it exists) onto the scene
	void Render();
	// This function frees the memory allocated by the Mesh
	void ClearMesh();

	// Destructor
	~Mesh();
};

