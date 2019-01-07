#version 330

#define MAX_POINT_LIGHTS	3
#define MAX_SPOT_LIGHTS		3

in vec3 vert_normal;
in vec2 vert_mainTex;
in vec3 vert_pos;
in vec4 vert_directionalLightSpacePos;

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

struct PointLight 
{
	Light light;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight 
{
	PointLight light;
	vec3 direction;
	float edge;
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

struct OmniShadowMap {
	samplerCube shadowMap;
	float farPlane;
};

uniform	Material u_material;
uniform vec3 u_cameraPosition;

uniform float u_ambientFactor;
uniform DirectionalLight u_directionalLight;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform int u_pointLightsCount = 0;
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];
uniform int u_spotLightsCount = 0;

uniform sampler2D u_mainTexture;
uniform sampler2D u_directionalStaticSM;
uniform sampler2D u_directionalDynamicSM;
uniform OmniShadowMap u_omniSM[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform samplerCube u_worldReflection;
uniform float u_reflectionFactor;

float CalculateOmniShadowFactor(PointLight light, int shadowMapIndex) {
	vec3 fragToLight = vert_pos - light.position;
	float currentDepth = length(fragToLight);
	
	float bias   = 0.15;

	float closestDepth = texture(u_omniSM[shadowMapIndex].shadowMap, fragToLight).r;
	closestDepth *= u_omniSM[shadowMapIndex].farPlane; 
	return float(currentDepth - bias > closestDepth);
}

float CalculateDirectionalShadowFactor(DirectionalLight light) 
{
	// Normalized coordinates
	vec3 projCoords = vert_directionalLightSpacePos.xyz / vert_directionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float currentDepth = projCoords.z;

	vec3 normal = normalize(vert_normal);
	vec3 lightDir = normalize(light.direction);

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = 0.0;

	vec2 s_texelSize = 1.0 / textureSize(u_directionalStaticSM, 0);
	vec2 d_texelSize = 1.0 / textureSize(u_directionalDynamicSM, 0);
	for(int x = -1; x < 1; x++) {
		for(int y = -1; y < 1; y++) {
			float s_pcfDepth = texture(u_directionalStaticSM, projCoords.xy + vec2(x, y) * s_texelSize).x;
			float d_pcfDepth = texture(u_directionalDynamicSM, projCoords.xy + vec2(x, y) * d_texelSize).x;
			shadow += max(float(currentDepth - bias > s_pcfDepth), float(currentDepth - bias > d_pcfDepth));
		}
	}
	shadow /= 9.0;

	return float(projCoords.z < 1.0) * shadow;
}

float CalculateAttenuation(float dist, float falloffStart, float falloffEnd)
{
	float attenuation = (falloffEnd - dist) / (falloffEnd - falloffStart);
    return clamp(attenuation, 0.0, 1.0);
}

vec4 CalculateLighting(FragParams frag, vec3 matColor, float matShininess, vec3 nLightToFrag, Light light, float shadowFactor) {
    vec4 outColor = vec4(0.0, 0.0, 0.0, 1.0);

    //Intensity of the diffuse light. Saturate to keep within the 0-1 range.
    float NdotL = dot( frag.frag_Normal, nLightToFrag );
    float intensity = clamp( NdotL, 0.0, 1.0 );

    // Calculate the diffuse light factoring in light color, power and the attenuation
    outColor += intensity * vec4(light.diffuseColor, 1.0f) * vec4(light.diffuseFactor, 1.0f);

    //Calculate the half vector between the light vector and the view vector.
    //This is typically slower than calculating the actual reflection vector
    // due to the normalize function's reciprocal square root
    vec3 H = normalize( nLightToFrag + frag.frag_nvToCam );

    //Intensity of the specular light
    float NdotH = dot( frag.frag_Normal, H );
    intensity = pow( clamp( NdotH, 0.0, 1.0 ), matShininess );

    //Sum up the specular light factoring
    outColor += intensity * vec4(light.specularColor, 1.0) * vec4(light.specularFactor, 1.0);

    return (1.0 - shadowFactor) * clamp(outColor * vec4(matColor, 1.0), 0.0, 1.0);
}
 
vec4 CalculateDirectionalLight(FragParams frag, vec3 matColor, float matShininess, DirectionalLight light) {
	return CalculateLighting(frag, u_material.albedo, u_material.shininess, -u_directionalLight.direction, u_directionalLight.light, CalculateDirectionalShadowFactor(light));
}

vec4 CalculatePointLight(FragParams frag, vec3 matColor, float matShininess, PointLight light, int shadowMapIndex) {
	vec3 lightToFrag = light.position - frag.frag_Position;
	float dLightToFrag = length(lightToFrag);
	vec3 nLightToFrag = normalize(lightToFrag);

	float shadowFactor = CalculateOmniShadowFactor(light, shadowMapIndex);
	vec4 plColor = CalculateLighting(frag, u_material.albedo, u_material.shininess, nLightToFrag, light.light, shadowFactor);

	// Calculate attenuation based on distance
	float attenuation = light.exponent * dLightToFrag * dLightToFrag + light.linear * dLightToFrag + light.constant;

	return plColor / attenuation;
}


vec4 CalculatePointLights(FragParams frag, vec3 matColor, float matShininess, PointLight lights[MAX_POINT_LIGHTS], int pointLightCount) {
	vec4 plsColor = vec4(0.0, 0.0, 0.0, 1.0);
	for(int i = 0; i < pointLightCount; i++) {
		plsColor += CalculatePointLight(frag, matColor, matShininess, lights[i], i);
	}

	return plsColor;
}

vec4 CalculateSpotLight(FragParams frag, vec3 matColor, float matShininess, float slFactor, SpotLight light, int shadowMapIndex) {
	vec4 color = CalculatePointLight(frag, matColor, matShininess, light.light, shadowMapIndex);
	return color * (1.0 - (1.0 - slFactor) * (1.0 / (1.0 - light.edge)));
}

vec4 CalculateSpotLights(FragParams frag, vec3 matColor, float matShininess, SpotLight lights[MAX_POINT_LIGHTS], int spotLightCount) {
	vec4 plsColor = vec4(0.0, 0.0, 0.0, 1.0);
	for(int i = 0; i < spotLightCount; i++) {
		vec3 rayDirection = normalize(frag.frag_Position - lights[i].light.position);
		float slFactor = dot(rayDirection, lights[i].direction);
		if(slFactor > lights[i].edge) {
			plsColor += CalculateSpotLight(frag, matColor, matShininess, slFactor, lights[i], i + u_pointLightsCount);
		}
	}

	return plsColor;
}

void main()
{
	vec4 textColor = texture(u_mainTexture, vert_mainTex);
	if(textColor.a <= 0.5)
		discard;

	FragParams frag;
	frag.frag_Position = vert_pos;
	frag.frag_Normal = -vert_normal;
	frag.frag_nvToCam = normalize(u_cameraPosition - vert_pos);
	
	vec4 dlColor = CalculateDirectionalLight(frag, u_material.albedo, u_material.shininess, u_directionalLight);
	vec4 plsColor = CalculatePointLights(frag, u_material.albedo, u_material.shininess, u_pointLights, u_pointLightsCount);
	vec4 slsColor = CalculateSpotLights(frag, u_material.albedo, u_material.shininess, u_spotLights, u_spotLightsCount);
	vec4 aColor = vec4(u_ambientFactor, u_ambientFactor, u_ambientFactor, 1.0);
	
	vec4 fColor = clamp( dlColor + plsColor + slsColor + aColor, 0.0, 1.0 );
	
    vec3 posToCam = normalize(vert_pos - u_cameraPosition);
    vec3 reflectVec = reflect(posToCam, -vert_normal);
    vec4 reflectColor = vec4(texture(u_worldReflection, reflectVec).rgb, 1.0);
	frag_color = (1.0 - u_reflectionFactor) * textColor * fColor + u_reflectionFactor * reflectColor;
}
