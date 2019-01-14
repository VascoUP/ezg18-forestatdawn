#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

class SkyBox
{
private:
	Mesh* mMesh;
	SkyBoxShader* mShader;

	GLuint mTextureID;
public:
	SkyBox();
	SkyBox(std::vector<std::string>* p_faceLocation);

	void Draw(glm::mat4* viewMatrix, glm::mat4* projectionMatrix);

	~SkyBox();
};

