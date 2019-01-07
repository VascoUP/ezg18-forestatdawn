#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "ShaderCompiler.h"
#include "ErrorShader.h"
#include "Material.h"
#include "Light.h"
#include "Commons.h"

class Shader
{
protected:
	// ID of the shader
	GLuint shaderID;
public:
	GLuint GetShaderID() const { return shaderID; }
	// Activates this shader whenever it's called
	void UseShader();
	// Frees the memory allocated by the shader program
	void ClearShader();
protected:
	// Reads shader code from a file
	std::string ReadFile(const char* file);

	GLuint GetUniformLocation(const char* uniform);

	virtual void GetShaderUniforms() = 0;

public:
	// Destructor
	~Shader();
};

class StandardShader :
	public Shader
{
public:
	// Given the vertex and fragment shader code it creates a shader program
	bool CreateFromString(const char* vertexCode, const char* fragmentCode, const char* geometryCode = nullptr);
	// Given the vertex and fragment shader files it creates a shader program
	bool CreateFromFiles(const char* vertexFile, const char* fragmentFile, const char* geometryFile = nullptr);
};

class DefaultShader :
	public StandardShader
{
private:
	// -- Transformation --
	// Projection matrix
	GLuint uniformProjection;
	// View matrix
	GLuint uniformView;
	// Model matrix
	GLuint uniformModel;

	// -- Camera --
	// Camera Position
	GLuint uniformCameraPosition;

	// -- Texture --
	// Texture
	GLuint uniformTexture;

	// -- Ambiente Light --
	// Ambient light intensity
	GLuint uniformAmbientIntensity;


	// -- Directional light --
	struct {
		// Directional light diffuse color
		GLuint uniformDiffuseColor;
		// Directional light diffuse factor
		GLuint uniformDiffuseFactor;
		// Directional light diffuse color
		GLuint uniformSpecularColor;
		// Directional light diffuse factor
		GLuint uniformSpecularFactor;
		// Directional light direction
		GLuint uniformDirection;
	} uniformDirectionalLight;
	GLuint uniformDirectionalLightTransform;
	GLuint uniformDirectionalStaticSM;
	GLuint uniformDirectionalDynamicSM;

	// -- Point Lights --
	int pointLightCount;
	GLuint uniformPointLightCount;
	struct {
		GLuint uniformDiffuseColor;
		GLuint uniformDiffuseFactor;
		GLuint uniformSpecularColor;
		GLuint uniformSpecularFactor;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLights[MAX_POINT_LIGHTS];

	int spotLightCount;
	GLuint uniformSpotLightCount;
	struct {
		GLuint uniformDiffuseColor;
		GLuint uniformDiffuseFactor;
		GLuint uniformSpecularColor;
		GLuint uniformSpecularFactor;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLights[MAX_SPOT_LIGHTS];

	// -- Shadow maps --
	struct {
		GLuint uniformStaticShadowMap;
		GLuint uniformDynamicShadowMap;
	};
	// Todo: ^ Use this ^

	// -- Omnidirectional shadow maps --
	struct {
		GLuint uniformShadowMap;
		GLuint uniformFarPlane;
	} uniformOmniSM[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	// -- Material --
	struct {
		GLuint uniformSpecularIntensity;
		GLuint uniformShininess;
		GLuint uniformAlbedo;
	} uniformMaterial;

	// -- Reflection --
	GLuint uniformWorldReflection;
	GLuint uniformReflectionFactor;

public:
	DefaultShader();

	// Getter for uniformModel
	GLuint GetModelLocation();

	void SetProjectionMatrix(glm::mat4 * pMatrix);
	void SetViewMatrix(glm::mat4 * vMatrix);
	void SetCameraPosition(glm::vec3 * cPosition);
	void SetAmbientIntensity(GLfloat aIntensity);
	void SetDirectionalLight(DirectionalLight* light);
	void SetPointLights(PointLight** pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight** sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetMaterial(Material* mat);
	void SetTexutre(GLuint textureUnit);
	void SetDirectionalStaticSM(GLuint textureUnit);
	void SetDirectionalDynamicSM(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4 * lTransform);
	void SetWorldReflection(GLuint textureUnit);
	void SetReflectionFactor(GLfloat factor);

protected:
	void GetShaderUniforms();
};

class CubeMapRenderShader :
	public StandardShader {
private:
	// -- Transformation --
	GLuint uniformViewProjectionMatrices[6];
	// Model matrix
	GLuint uniformModel;

	// -- Camera --
	// Camera Position
	GLuint uniformCameraPosition;

	// -- Texture --
	// Texture
	GLuint uniformTexture;

	// -- Ambiente Light --
	// Ambient light intensity
	GLuint uniformAmbientIntensity;

	// -- Directional light --
	struct {
		// Directional light diffuse color
		GLuint uniformDiffuseColor;
		// Directional light diffuse factor
		GLuint uniformDiffuseFactor;
		// Directional light diffuse color
		GLuint uniformSpecularColor;
		// Directional light diffuse factor
		GLuint uniformSpecularFactor;
		// Directional light direction
		GLuint uniformDirection;
	} uniformDirectionalLight;

	// -- Point Lights --
	int pointLightCount;
	GLuint uniformPointLightCount;
	struct {
		GLuint uniformDiffuseColor;
		GLuint uniformDiffuseFactor;
		GLuint uniformSpecularColor;
		GLuint uniformSpecularFactor;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLights[MAX_POINT_LIGHTS];

	int spotLightCount;
	GLuint uniformSpotLightCount;
	struct {
		GLuint uniformDiffuseColor;
		GLuint uniformDiffuseFactor;
		GLuint uniformSpecularColor;
		GLuint uniformSpecularFactor;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLights[MAX_SPOT_LIGHTS];

	// -- Material --
	struct {
		GLuint uniformSpecularIntensity;
		GLuint uniformShininess;
		GLuint uniformAlbedo;
	} uniformMaterial;

public:
	CubeMapRenderShader();

	// Getter for uniformModel
	GLuint GetModelLocation();

	void SetViewProjectMatrices(std::vector<glm::mat4> lightMatrices);
	void SetCameraPosition(glm::vec3 * cPosition);
	void SetAmbientIntensity(GLfloat aIntensity);
	void SetDirectionalLight(DirectionalLight* light);
	void SetPointLights(PointLight** pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight** sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetMaterial(Material* mat);
	void SetTexutre(GLuint textureUnit);

protected:
	void GetShaderUniforms();
};

class DirectionalShadowMapShader :
	public StandardShader
{
private:
	GLuint uniformModel;
	GLuint uniformDirectionalLightTransform;
public:
	DirectionalShadowMapShader();

	GLuint GetModelLocation();

	void SetModel(glm::mat4* mMatrix);
	void SetDirectionalLightTransform(glm::mat4* lTransform);

protected:
	void GetShaderUniforms();
};

class OmnidirectionalShadowMapShader :
	public StandardShader
{
private:
	GLuint uniformModel;
	GLuint uniformLightMatrices[6];
	GLuint uniformLightPos;
	GLuint uniformFarPlane;

public:
	OmnidirectionalShadowMapShader();

	GLuint GetModelLocation();

	void SetModel(glm::mat4* mMatrix);
	void SetLightPosition(glm::vec3* lPos);
	void SetFarPlane(GLfloat far);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices);

protected:
	void GetShaderUniforms();
};

class CustomShader 
	: public StandardShader
{
public:
	CustomShader();

	GLuint GetShaderID();

protected:
	void GetShaderUniforms();
};

class ComputeShader :
	public Shader {
public:
	bool CreateFromString(const char* computeCode);
};

class JoinDepthBuffersShader 
	: public ComputeShader
{
	/*
	#version 430

	uniform vec4 texIncrement;
	uniform image2D stcTex;
	uniform image2D dnmTex;
	uniform image2D dstTex;

	void main() {
		ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
		data = ?
		imageStore(dstTex, storePos, data);\
	}
	*/
};