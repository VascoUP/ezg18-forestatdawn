#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Window dimension
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265359f / 180.0f;

GLuint VAO, VBO, EBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float currRot = 0.0f;
float deltaRot = 0.05f;

// Vertex Shader code
static const char* vShader = "										\n\
#version 330														\n\
																	\n\
layout (location = 0) in vec3 pos;									\n\
																	\n\
out vec4 vCol;														\n\
																	\n\
uniform mat4 model;													\n\
																	\n\
void main() {														\n\
	gl_Position = model * vec4(pos.xyz, 1.0);						\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);						\n\
}";

// Fragment Shader
static const char* fShader = "										\n\
#version 330														\n\
																	\n\
in vec4 vCol;														\n\
																	\n\
out	vec4 colour;													\n\
																	\n\
void main() {														\n\
	colour = vCol;													\n\
}";

// Err Vertex Shader
static const char* errVShader = "									\n\
#version 330														\n\
																	\n\
layout(location = 0) in vec3 pos;									\n\
																	\n\
void main() {														\n\
	gl_Position = vec4(pos, 1.0);									\n\
}";

// Err Fragment Shader
static const char* errFShader = "									\n\
#version 330														\n\
																	\n\
out	vec4 colour;													\n\
																	\n\
void main() {														\n\
	colour = vec4(1.0, 0.0, 1.0, 1.0);								\n\
}";

void CreateTriangle() {
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return false;
	}

	glAttachShader(theProgram, theShader);

	return true;
}

bool CompileShaders() {
	shader = glCreateProgram();

	if (!shader) {
		printf("Error creating shader program");
		return false;
	}

	if (!AddShader(shader, vShader, GL_VERTEX_SHADER)) {
		return false;
	}
	if (!AddShader(shader, fShader, GL_FRAGMENT_SHADER)) {
		return false;
	}

	GLint result = 0;
	GLchar oLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(oLog), NULL, oLog);
		printf("Error linking program: '%s'\n", oLog);

		//	Setup error shader on mesh
		AddShader(shader, errVShader, GL_VERTEX_SHADER);
		AddShader(shader, errFShader, GL_FRAGMENT_SHADER);
		glLinkProgram(shader);
		glValidateProgram(shader);
		return false;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(oLog), NULL, oLog);
		printf("Error validating program: '%s'\n", oLog);

		//	Setup error shader on mesh
		AddShader(shader, errVShader, GL_VERTEX_SHADER);
		AddShader(shader, errFShader, GL_FRAGMENT_SHADER);
		glLinkProgram(shader);
		glValidateProgram(shader);
		return false;
	}

	uniformModel = glGetUniformLocation(shader, "model");

	return true;
}

bool ErrShader() {
	shader = glCreateProgram();

	if (!shader) {
		printf("Error creating shader program");
		return false;
	}

	AddShader(shader, errVShader, GL_VERTEX_SHADER);
	AddShader(shader, errFShader, GL_FRAGMENT_SHADER);
	glLinkProgram(shader);
	glValidateProgram(shader);

	return true;
}

bool GetShaders() {
	if (!CompileShaders()) {
		if (!ErrShader()) {
			return false;
		}
	}
	return true;
}

int main() {
	// Initialize GLFW
	if (!glfwInit()) {
		printf("GLFW initialization failed.");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwared Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLU_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed.");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for glfw to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	if (!GetShaders()) {
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
	}
	
	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {
		// Get + Handle user input events
		glfwPollEvents();

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		currRot += deltaRot;

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);
		
		glm::mat4 model;
		//model = glm::translate(model, glm::vec3(triOffset, triOffset, 0.0f));
		model = glm::rotate(model, currRot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}