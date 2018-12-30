#pragma once

#include <GL\glew.h>
#include "stb_image.h"

class Texture
{
private:
	GLuint textureID;

	const char* fileLocation;
public:
	int width, height, bitDepth;
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();

	void UseTexture();
	void ClearTexture();

	~Texture();
};

