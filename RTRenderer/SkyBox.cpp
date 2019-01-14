#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::SkyBox(std::vector<std::string>* p_faceLocation)
{
	mShader = new SkyBoxShader();
	mShader->CreateFromFiles("Shaders/skybox.vert", "Shaders/skybox.frag");

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	int width, height, bitDepth;

	for (size_t i = 0; i < 6; i++)
	{
		unsigned char *texData = stbi_load((*p_faceLocation)[i].c_str(), &width, &height, &bitDepth, 0);
		if (!texData)
		{
			printf("Failed to find: %s\n", (*p_faceLocation)[i].c_str());
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Mesh Setup
	unsigned int indices[] = {
		// front
		2, 1, 0,
		3, 1, 2,
		// right
		5, 3, 2,
		7, 3, 5,
		// back
		4, 7, 5,
		6, 7, 4,
		// left
		0, 6, 4,
		1, 6, 0,
		// top
		5, 0, 4,
		2, 0, 5,
		// bottom
		3, 6, 1,
		7, 6, 3
	};

	float vertices[] = {
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	MeshInfo info;
	info.vertices = vertices; info.indices = indices;
	info.numOfVertices = 64; info.numOfIndices = 36;

	mMesh = new Mesh(&info);
	mMesh->Load();
}

void SkyBox::Draw(glm::mat4 * viewMatrix, glm::mat4 * projectionMatrix)
{
	glDepthMask(GL_FALSE);

	mShader->UseShader();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	mShader->SetSkyBox(1);

	// Ensure that there are no translation on the camera
	*viewMatrix = glm::mat4(glm::mat3(*viewMatrix));
	*projectionMatrix = (*projectionMatrix) * (*viewMatrix);
	mShader->SetViewProjectionMatrix(projectionMatrix);

	ShaderCompiler::ValidateProgram(mShader->GetShaderID());

	mMesh->Render();

	glDepthMask(GL_TRUE);
}

SkyBox::~SkyBox()
{
	if (mTextureID)
		glDeleteTextures(1, &mTextureID);
}
