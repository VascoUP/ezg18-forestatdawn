#version 330

#define MAX_POINT_LIGHTS	3
#define MAX_SPOT_LIGHTS		3

in vec3 vert_normal;
in vec2 vert_mainTex;
in vec3 vert_pos;

out vec4 frag_color;

struct Light {
	vec3 diffuseColor;
	vec3 diffuseFactor;
	vec3 specularColor;
	vec3 specularFactor;
};

struct DirectionalLight 
{
	Light light;
    vec3 direction;
};

struct SpotLight 
{
	Light light;
	vec3 position;
	vec3 direction;
	float fallOffStart;
	float fallOffEnd;
};

struct PointLight 
{
	Light light;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

// --New lighting model
struct FragParams {
	// Fragment position
	vec3 frag_Position;
	// Fragment normal
	vec3 frag_Normal;
	// Normalized vector (nv) from camera to fragment position
	vec3 frag_nvToCam;
};

struct Material {
	float specularIntensity;
	float shininess;
	vec3 albedo;
};

uniform sampler2D u_mainTexture;
uniform	Material u_material;
uniform vec3 u_cameraPosition;

uniform float u_ambientFactor;
uniform DirectionalLight u_directionalLight;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform int u_pointLightsCount = 0;
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];
uniform int u_spotLightsCount = 0;


float CalculateAttenuation(float dist, float falloffStart, float falloffEnd)
{
	float attenuation = (falloffEnd - dist) / (falloffEnd - falloffStart);
    return clamp(attenuation, 0.0f, 1.0f);
}

vec4 CalculateLighting(FragParams frag, vec3 matColor, float matShininess, vec3 nLightToFrag, Light light) {
    vec4 outColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    //Intensity of the diffuse light. Saturate to keep within the 0-1 range.
    float NdotL = dot( frag.frag_Normal, nLightToFrag );
    float intensity = clamp( NdotL, 0.0f, 1.0f );

    // Calculate the diffuse light factoring in light color, power and the attenuation
    outColor += intensity * vec4(light.diffuseColor, 1.0f) * vec4(light.diffuseFactor, 1.0f);

    //Calculate the half vector between the light vector and the view vector.
    //This is typically slower than calculating the actual reflection vector
    // due to the normalize function's reciprocal square root
    vec3 H = normalize( nLightToFrag + frag.frag_nvToCam );

    //Intensity of the specular light
    float NdotH = dot( frag.frag_Normal, H );
    intensity = pow( clamp( NdotH, 0.0f, 1.0f ), matShininess );

    //Sum up the specular light factoring
    outColor += intensity * vec4(light.specularColor, 1.0f) * vec4(light.specularFactor, 1.0f);

    return outColor * vec4(matColor, 1.0f);
}

vec4 CalculateDirectionalLight(FragParams frag, vec3 matColor, float matShininess, DirectionalLight light) {
	return CalculateLighting(frag, u_material.albedo, u_material.shininess, -u_directionalLight.direction, u_directionalLight.light);
}

vec4 CalculatePointLight(FragParams frag, vec3 matColor, float matShininess, PointLight light) {
	vec3 lightToFrag = light.position - frag.frag_Position;
	float dLightToFrag = length(lightToFrag);
	lightToFrag = normalize(lightToFrag);

	vec4 plColor = CalculateLighting(frag, u_material.albedo, u_material.shininess, lightToFrag, light.light);

	// Calculate attenuation based on distance
	float attenuation = light.exponent * dLightToFrag * dLightToFrag + light.linear * dLightToFrag + light.constant;

	return plColor / attenuation;
}

vec4 CalculatePointLights(FragParams frag, vec3 matColor, float matShininess, PointLight lights[MAX_POINT_LIGHTS], int pointLightCount) {
	vec4 plsColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < pointLightCount; i++) {
		plsColor += CalculatePointLight(frag, matColor, matShininess, lights[i]);
	}

	return plsColor;
}

void main()
{
	FragParams frag;
	frag.frag_Position = vert_pos;
	frag.frag_Normal = -vert_normal;
	frag.frag_nvToCam = normalize(u_cameraPosition - vert_pos);
	
	vec4 dlColor = CalculateDirectionalLight(frag, u_material.albedo, u_material.shininess, u_directionalLight);
	vec4 plsColor = CalculatePointLights(frag, u_material.albedo, u_material.shininess, u_pointLights, u_pointLightsCount);
	vec4 aColor = vec4(u_ambientFactor, u_ambientFactor, u_ambientFactor, 1.0f);
	
	frag_color = texture(u_mainTexture, vert_mainTex) * clamp( dlColor + plsColor + aColor, 0.0f, 1.0f );
}
