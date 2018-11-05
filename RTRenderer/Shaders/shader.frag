#version 330

#define SPOTLIGHT_LENGTH	6	
#define POINTLIGHT_LENGTH	6

in vec3 fragNormal;
in vec2 fragMainTex;
in vec3 fragPos;

out vec4 color;

struct DirectionalLight 
{
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float intensity;
};

struct SpotLight {
	vec3 color;
	vec3 position;
	vec3 direction;
	float angle;
	float intensity;
};

struct PointLight {
	vec3 color;
	vec3 position;
	float intensity;
};

struct Material {
	float specularIntensity;
	float shininess;
};

uniform DirectionalLight directionalLight;
uniform SpotLight spotLights[SPOTLIGHT_LENGTH];
uniform int spotLightsCount = 0;
uniform PointLight pointLights[POINTLIGHT_LENGTH];
uniform int pointLightsCount = 0;

uniform sampler2D mainTexture;
uniform	Material material;
uniform vec3 cameraPosition;

vec4 directionalColor(DirectionalLight light, vec3 nNormal) {
	float diffuseFactor = max(dot(nNormal, normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColor = vec4(directionalLight.color * directionalLight.intensity * diffuseFactor, 1.0f);
	
	float specularFactor = 0;
	vec4 specularColor = vec4(0,0,0,0);
	
	if(diffuseFactor > 0.0f) {
		vec3 fragToCamera = normalize(cameraPosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, nNormal));
		float specularFactor = max(dot(fragToCamera, reflectedVertex), 0.0f);

		if(specularFactor > 0.0f) {
			specularFactor += pow(specularFactor, material.shininess);
			specularColor += vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return specularColor + diffuseColor;
}

vec4 pointLightColor(PointLight light, vec3 nNormal) {
	vec3 pLightToObj = fragPos - light.position;
	vec3 nPLightToObj = normalize(pLightToObj);
	float diffuseFactor = max(dot(nNormal, nPLightToObj), 0.0f);
	vec4 diffuseColor = vec4(light.color * diffuseFactor * light.intensity / length(pLightToObj), 1.0f);
		
	vec3 fragToCamera = normalize(cameraPosition - fragPos);
	vec3 reflectedVertex = normalize(reflect(nPLightToObj, nNormal));
	float specularFactor = max(dot(fragToCamera, reflectedVertex), 0.0f);
	vec4 specularColor = vec4(0,0,0,0);

	if(specularFactor > 0.0f) {
		specularFactor += pow(specularFactor, material.shininess);
		specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
	}

	return diffuseColor + specularColor;
}

vec4 spotLightColor(SpotLight light, vec3 nNormal) {
	vec4 diffuseColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	vec3 pLightToObj = fragPos - light.position;
	vec3 nPLightToObj = normalize(pLightToObj);

	// If inside the spot light cone
	if(dot(nNormal, nPLightToObj) > light.angle) {
		float diffuseFactor = max(dot(nNormal, nPLightToObj), 0.0f);
		diffuseColor += vec4(light.color * diffuseFactor * light.intensity, 1.0f);
		
		vec3 fragToCamera = normalize(cameraPosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(nPLightToObj, nNormal));
		float specularFactor = max(dot(fragToCamera, reflectedVertex), 0.0f);

		if(specularFactor > 0.0f) {
			specularFactor += pow(specularFactor, material.shininess);
			specularColor += vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return diffuseColor + specularColor;
}

vec4 lightingFragColor() {
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	color += vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

	vec3 nNormal = normalize(-fragNormal);

	color += directionalColor(directionalLight, nNormal);
	
	// Point Lights
	for(int i = 0; i < pointLightsCount; i++) {
		color += pointLightColor(pointLights[i], nNormal);
	}

	// Spot Lights
	for(int i = 0; i < spotLightsCount; i++) {
		color += spotLightColor(spotLights[i], nNormal);
	}

	return color;
}

void main()
{
	color = texture(mainTexture, fragMainTex) * lightingFragColor();
}