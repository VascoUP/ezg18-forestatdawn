#version 330

#define SPOTLIGHT_LENGTH	6	
#define POINTLIGHT_LENGTH	6

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
	float fallOffStart;
	float fallOffEnd;
};

struct PointLight {
	vec3 color;
	vec3 position;
	float intensity;
	float fallOffStart;
	float fallOffEnd;
};

struct Material {
	float specularIntensity;
	float shininess;
};

struct FragParams {
	// Fragment position
	vec3 frag_Position;
	// Fragment normal
	vec3 frag_Normal;
	// Normalized vector (nv) from camera to fragment position
	vec3 frag_nvToCam;
};

float CalculateAttenuation(float dist, float falloffStart, float falloffEnd)
{
	float attenuation = (falloffEnd - dist) / (falloffEnd - falloffStart);

    return clamp(attenuation, 0.0f, 1.0f);
}

vec3 SchlickFresnel(vec3 r0, vec3 normal, vec3 lightDirection)
{
    float cosIncidentAngle = clamp(dot(normal, lightDirection), 0.0f, 1.0f);
    float f0 = 1.0f - cosIncidentAngle;

    return r0 + (1.0f - r0) * (f0 * f0 * f0 * f0 * f0);
}

vec4 ComputeBlinnPhong(vec3 lightStrength, vec3 lightDirection, vec3 normal, vec3 toEyeDirection, Material material)
{
    // Derive m from the shininess, which is derived from the roughness:
	float m = material.shininess * 256.0f;

	// Calculate the halfway vector:
	vec3 halfwayVector = normalize(toEyeDirection + lightDirection);

	// Calculate the specular albedo:
	float roughnessFactor = (m + 8.0f) * pow(max(dot(halfwayVector, normal), 0.0f), m) / 8.0f;
	vec3 fresnelFactor = SchlickFresnel(vec3(1.0f, 1.0f, 1.0f), normal, lightDirection);
	vec3 specularAlbedo = fresnelFactor * roughnessFactor;

	// The equation used goes off the [0, 1] range, so it needs to be scaled:
	specularAlbedo = specularAlbedo / (specularAlbedo + 1.0f);

	return vec4((specularAlbedo) * lightStrength, 1.0f);
}

vec4 ComputeDirectionalLight(DirectionalLight light, FragParams frag, Material material) {
	float lightStrength = light.intensity * max(dot(frag.frag_Normal, normalize(light.direction)), 0.0f);
	return ComputeBlinnPhong(light.color * lightStrength, -light.direction, frag.frag_Normal, frag.frag_nvToCam, material);
}

vec4 ComputePointLight(PointLight light, FragParams frag, Material material) {
	vec3 vFragLight = frag.frag_Position - light.position;
	vec3 nvFragLight = normalize(vFragLight);
    float dFragLight = length(vFragLight);

    if (dFragLight > light.fallOffEnd)
        return vec3(0.0f, 0.0f, 0.0f);

	// Scale light by Lambert's cosine law:
    vec3 lightStrength = light.color * light.intensity * max(dot(frag.frag_Normal, nvFragLight), 0.0f);

	// Apply attenuation
    float attenuation = CalculateAttenuation(dFragLight, light.fallOffStart, light.fallOffEnd);
    lightStrength *= attenuation;
	
    return ComputeBlinnPhong(lightStrength, nvFragLight, frag.frag_Normal, frag.frag_nvToCam, material);
}

vec4 ComputeSpotLight(SpotLight light, FragParams frag, Material material) {		
	vec3 vFragLight = frag.frag_Position - light.position;
	vec3 nvFragLight = normalize(vFragLight);
    float dFragLight = length(vFragLight);

    if (dFragLight > light.fallOffEnd)
        return vec3(0.0f, 0.0f, 0.0f);
	
	// Scale light by Lambert's cosine law:
    vec3 lightStrength = light.color * light.intensity * max(dot(frag.frag_Normal, nvFragLight), 0.0f);

	// Attenuate light
    float attenuation = CalculateAttenuation(dFragLight, light.fallOffStart, light.fallOffEnd);
    lightStrength *= attenuation;

	// Scale by spot light:
	vec3 nvReflected = normalize(reflect(nvFragLight, frag.frag_Normal));
	float sf = max(dot(frag.frag_nvToCam, nvReflected), 0.0f);
	if(sf > 0.0f) {
		sf += pow(sf, material.shininess);
	}
    lightStrength *= sf;
	
    return ComputeBlinnPhong(lightStrength, nvFragLight, frag.frag_Normal, frag.frag_nvToCam, material);
}

vec4 ComputeLighting(DirectionalLight dLight, PointLight pointLights[POINTLIGHT_LENGTH], SpotLight spotLights[SPOTLIGHT_LENGTH], FragParams frag, Material material) {
	// --Directional light
	vec4 color = ComputeDirectionalLight(dLight, frag, material);

	// --Point lights
	for(int i = 0; i < POINTLIGHT_LENGTH; i++) {
		color += ComputePointLight(pointLights[i], frag, material);
	}

	// --Spot lights
	for(int i = 0; i < SPOTLIGHT_LENGTH; i++) {
		color += ComputeSpotLight(spotLights[i], frag, material);		
	}

	return clamp(color, 0.0f, 1.0f);
}