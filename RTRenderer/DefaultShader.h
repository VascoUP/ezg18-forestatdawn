#pragma once

#include "Shader.h"

class DefaultShader :
	public Shader
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

	// -- Material --
	struct {
		GLuint uniformSpecularIntensity;
		GLuint uniformShininess;
		GLuint uniformAlbedo;
	} uniformMaterial;

public:
	DefaultShader();

	// Getter for uniformModel
	GLuint GetModelLocation();

	void SetProjectionMatrix(glm::mat4 * pMatrix);
	void SetViewMatrix(glm::mat4 * vMatrix);
	void SetCameraPosition(glm::vec3 * cPosition);
	void SetAmbientIntensity(GLfloat aIntensity);
	void SetDirectionalLight(DirectionalLight* light);
	void SetPointLights(PointLight** pLight, unsigned int lightCount);
	void SetSpotLights(SpotLight** sLight, unsigned int lightCount);
	void SetMaterial(Material* mat);
	void SetTexutre(GLuint textureUnit);
	void SetDirectionalStaticSM(GLuint textureUnit);
	void SetDirectionalDynamicSM(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4 * lTransform);

protected:
	void GetShaderUniforms();
};

