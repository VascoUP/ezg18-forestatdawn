#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <sstream>

#include "GLProgram.h"

using namespace std;

int main() {
	RenderMode mode = RenderMode::UNDEFINED;
	while (mode == RenderMode::UNDEFINED) {
		printf("Which mode would you like to run?\n1. Cinematic\n2. Free roam\nChoice: ");
		string inputStr;
		getline(cin, inputStr);
		if (inputStr.length() > 1)
			continue;
		if (inputStr[0] == '1')
			mode = RenderMode::CINEMATIC;
		else if (inputStr[0] == '2')
			mode = RenderMode::ROAM;
	}

	GLProgram* program = GLProgram::CreateGLProgramInstance(mode);
	program->Run();

	return 0;
}