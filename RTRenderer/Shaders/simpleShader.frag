#version 330

#define MAX_POINT_LIGHTS	3
#define MAX_SPOT_LIGHTS		3

// -- Lights -- 
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
// -- End lights --

// -- Shadow maps --
struct ShadowMap {
	sampler2D static_shadowmap;
	sampler2D dynamic_shadowmap;
};

struct OmniShadowMap {
	samplerCube static_shadowmap;
	float farPlane;
};
// -- End shadow maps --

// -- Materials
struct Material {
	float specularIntensity;
	float shininess;
	vec3 albedo;
	sampler2D albedoTexture;
};
// -- End materials --

// -- Calculations not to be repeated --
struct FragParams {
	// Fragment position
	vec3 frag_Position;
	// Fragment normal
	vec3 frag_Normal;
	// Normalized vector (nv) from camera to fragment position
	vec3 frag_nvToCam;
};
// -- End of this -- 

// -- In -- 
in vec3 vert_normal;
in vec2 vert_mainTex;
in vec3 vert_pos;
in vec4 vert_directionalLightSpacePos;
// -- End in --

// -- Out --
out vec4 frag_color;
// -- End out --

// -- Uniforms --
	// -- Updated per frame -- 
	uniform vec3 u_cameraPosition;					// Camera position world space

	uniform samplerCube u_skybox;					// Environment skybox
	uniform float u_ambientFactor;					// Factor of ambient lighting

	uniform DirectionalLight u_directionalLight;	// Direction light (only one per scene)
	uniform ShadowMap u_directionalSM;				// Directional shadow map

	uniform PointLight 
		u_pointLights[MAX_POINT_LIGHTS];			// Point lights (up until MAX_POINT_LIGHTS)
	uniform int u_pointLightsCount = 0;				// Number of point lights
	uniform SpotLight 
		u_spotLights[MAX_SPOT_LIGHTS];				// Spot lights (up until MAX_SPOT_LIGHTS)
	uniform int u_spotLightsCount = 0;				// Number of spot lights
	uniform OmniShadowMap 
		u_omniSM[MAX_POINT_LIGHTS+MAX_SPOT_LIGHTS];	// Omni shadow maps (has many as the number of point and spot lights
	// -- End update per frame --

	// -- Updated per object --
	uniform	Material u_material;					// Surface material

		// -- Rarely updated -- 
		uniform samplerCube u_worldReflection;			// Environmnet mapping (might be the skybox)
		uniform float u_reflectionFactor;				// Reflection factor of this surface
		uniform float u_refractionFactor;				// Refraction factoe of this surface
		uniform vec3 u_IoRValues;						// Refraction factors by color
		uniform vec3 u_fresnelValues;					// Fresnel parameters based on the surface
		// -- End rarely updated -- 
	// -- End updated per object -- 
// -- End uniforms --

// -- Calculation for shadow maps --
float CalculateOmniShadowFactor(PointLight light, int shadowMapIndex, vec3 fragToLight, float depth) {
//	for(float x = -offset; x < offset; x += offset / (samples * 0.5))
//	{
//	    for(float y = -offset; y < offset; y += offset / (samples * 0.5))
//	    {
//			for(float z = -offset; z < offset; z += offset / (samples * 0.5))
//			{
//				float closestDepth = texture(u_omniSM[shadowMapIndex].static_shadowmap, fragToLight + vec3(x,y,z)).r;
//				closestDepth *= u_omniSM[shadowMapIndex].farPlane;
//				shadow += float(depth > closestDepth);				
//			}
//		}
//	}

	float closestDepth = texture(u_omniSM[shadowMapIndex].static_shadowmap, fragToLight).r * u_omniSM[shadowMapIndex].farPlane;	
	return float(depth < u_omniSM[shadowMapIndex].farPlane) * float(depth > closestDepth);
}

float CalculateDirectionalShadowFactor(DirectionalLight light) 
{
	// Normalized coordinates
	vec3 projCoords = vert_directionalLightSpacePos.xyz / vert_directionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float currentDepth = projCoords.z;

//	vec3 normal = normalize(vert_normal);
//	vec3 lightDir = normalize(light.direction);

//	float shadow = 0.0;

//	vec2 s_texelSize = 1.0 / textureSize(u_directionalSM.static_shadowmap, 0);
//	vec2 d_texelSize = 1.0 / textureSize(u_directionalSM.dynamic_shadowmap, 0);
//	for(int x = -1; x < 1; x++) {
//		for(int y = -1; y < 1; y++) {
//			float s_pcfDepth = texture(u_directionalSM.static_shadowmap, projCoords.xy + vec2(x, y) * s_texelSize).x;
//			float d_pcfDepth = texture(u_directionalSM.dynamic_shadowmap, projCoords.xy + vec2(x, y) * d_texelSize).x;
//			shadow += max(float(currentDepth > s_pcfDepth), float(currentDepth > d_pcfDepth));
//		}
//	}
//	shadow /= 9.0;

	float sDepth = texture(u_directionalSM.static_shadowmap, projCoords.xy).x;
	float dDepth = texture(u_directionalSM.dynamic_shadowmap, projCoords.xy).x;

	return float(projCoords.z < 1.0) * max(float(currentDepth > sDepth), float(currentDepth > dDepth));
}
// -- End calculation for shadow maps --

// -- Calculation for lighting -- 
float CalculateAttenuation(float dist, float falloffStart, float falloffEnd)
{
	float attenuation = (falloffEnd - dist) / (falloffEnd - falloffStart);
    return clamp(attenuation, 0.0, 1.0);
}

vec4 CalculateLighting(FragParams frag, vec3 matColor, float matSpecIntensity, float matShininess, vec3 nLightToFrag, Light light, float shadowFactor) {
    vec4 outColor = vec4(0.0, 0.0, 0.0, 1.0);

	// Diffuse
    float NdotL = dot( nLightToFrag, frag.frag_Normal );
    float intensity = max( NdotL, 0.0 );

    outColor += intensity * vec4(light.diffuseColor, 1.0f) * vec4(light.diffuseFactor, 1.0f);

	// Specular 
    vec3 H = normalize( nLightToFrag + frag.frag_nvToCam );
    float NdotH = max( dot( H, frag.frag_Normal ), 0.0 );
    intensity = pow( NdotH, matShininess );
	
    outColor += intensity * matSpecIntensity * vec4(light.specularColor, 1.0) * vec4(light.specularFactor, 1.0);

    return vec4((1.0 - shadowFactor) * outColor.rgb, 1.0) * vec4(matColor, 1.0);
}
 
vec4 CalculateDirectionalLight(FragParams frag, DirectionalLight light) {
	float shadow = 0.0;//CalculateDirectionalShadowFactor(light);
	return CalculateLighting(
		frag, 
		u_material.albedo, 
		u_material.specularIntensity, 
		u_material.shininess, 
		-u_directionalLight.direction, 
		u_directionalLight.light, 
		shadow);
}

vec4 CalculatePointLight(FragParams frag, PointLight light, int shadowMapIndex) {
	vec3 lightToFrag = light.position - frag.frag_Position;
	float dLightToFrag = length(lightToFrag);
	vec3 nLightToFrag = normalize(lightToFrag);


	float shadowFactor = 0.0; //CalculateOmniShadowFactor(light, shadowMapIndex, -lightToFrag, dLightToFrag);
	vec4 plColor = CalculateLighting(frag, u_material.albedo, u_material.specularIntensity, u_material.shininess, nLightToFrag, light.light, shadowFactor);

	// Calculate attenuation based on distance
	float attenuation = light.exponent * dLightToFrag * dLightToFrag + light.linear * dLightToFrag + light.constant;

	return plColor / attenuation;
}


vec4 CalculatePointLights(FragParams frag, PointLight lights[MAX_POINT_LIGHTS], int pointLightCount) {
	vec4 plsColor = vec4(0.0, 0.0, 0.0, 1.0);
	for(int i = 0; i < pointLightCount; i++) {
		plsColor += CalculatePointLight(frag, lights[i], i);
	}

	return plsColor;
}

vec4 CalculateSpotLight(FragParams frag, float slFactor, SpotLight light, int shadowMapIndex) {
	vec4 color = CalculatePointLight(frag, light.light, shadowMapIndex);
	return color * (1.0 - (1.0 - slFactor) * (1.0 / (1.0 - light.edge)));
}

vec4 CalculateSpotLights(FragParams frag, SpotLight lights[MAX_POINT_LIGHTS], int spotLightCount) {
	vec4 plsColor = vec4(0.0, 0.0, 0.0, 1.0);
	for(int i = 0; i < spotLightCount; i++) {
		vec3 rayDirection = normalize(frag.frag_Position - lights[i].light.position);
		float slFactor = dot(rayDirection, lights[i].direction);
		if(slFactor > lights[i].edge) {
			plsColor += CalculateSpotLight(frag, slFactor, lights[i], i + u_pointLightsCount);
		}
	}

	return plsColor;
}


vec4 CalculateLigthing(FragParams frag) {
	vec4 dlColor = CalculateDirectionalLight(frag, u_directionalLight);
	vec4 plsColor = CalculatePointLights(frag, u_pointLights, u_pointLightsCount);
	vec4 slsColor = CalculateSpotLights(frag, u_spotLights, u_spotLightsCount);
	vec4 aColor = vec4(u_ambientFactor, u_ambientFactor, u_ambientFactor, 1.0);
	return dlColor + plsColor + slsColor + aColor;
}
// -- End calculation for lighting -- 

// -- Calculations for reflections and refractions -- 
float FresnelApproximation(float iDotN, vec3 fresnelValues)
{
    float bias = fresnelValues.x;
    float power = fresnelValues.y;
    float scale = fresnelValues.z;
    return bias + pow(1.0 - iDotN, power) * scale;
}

vec4 CalculateReflection(FragParams frag) {
	vec3 reflectVec = reflect(-frag.frag_nvToCam, frag.frag_Normal);
	vec3 refColor = texture(u_worldReflection, reflectVec).rgb;
	if(equal(refColor, vec3(0.0, 0.0, 0.0)) == bvec3(1, 1, 1)) {
		return vec4(texture(u_skybox, frag.frag_Normal).rgb, 1.0);
	}
	return vec4(refColor, 1.0);
}

vec4 CalculateRefraction(FragParams frag) {
	// -- Refraction color --
	vec3 refractColor;
	// Red
	vec3 refractTex = texture(u_worldReflection, refract(-frag.frag_nvToCam, frag.frag_Normal, u_IoRValues.x)).rgb;
	if(equal(refractTex, vec3(0.0, 0.0, 0.0)) == bvec3(1, 1, 1)) {
		refractColor.r = texture(u_skybox, refract(-frag.frag_nvToCam, frag.frag_Normal, u_IoRValues.x)).r;
	} else {
		refractColor.r = refractTex.r;
	}
	// Green
	refractTex = texture(u_worldReflection, refract(-frag.frag_nvToCam, frag.frag_Normal, u_IoRValues.y)).rgb;
	if(equal(refractTex, vec3(0.0, 0.0, 0.0)) == bvec3(1, 1, 1)) {
		refractColor.g = texture(u_skybox, refract(-frag.frag_nvToCam, frag.frag_Normal, u_IoRValues.y)).g;
	} else {
		refractColor.g = refractTex.g;
	}
	// Blue
	refractTex = texture(u_worldReflection, refract(-frag.frag_nvToCam, frag.frag_Normal, u_IoRValues.z)).rgb;
	if(equal(refractTex, vec3(0.0, 0.0, 0.0)) == bvec3(1, 1, 1)) {
		refractColor.b = texture(u_skybox, refract(-frag.frag_nvToCam, frag.frag_Normal, u_IoRValues.z)).b;
	} else {
		refractColor.b = refractTex.b;
	}
	return vec4(refractColor, 1.0);
}
// -- End calculations for reflections and refractions -- 


void main()
{
	vec4 tColor = texture(u_material.albedoTexture, vert_mainTex);
	if(tColor.a < 0.5)
		discard;

	FragParams frag;
	frag.frag_Position = vert_pos;
	frag.frag_Normal = -vert_normal;
	frag.frag_nvToCam = normalize(u_cameraPosition - vert_pos);
	
	// -- Color from lights --
	vec4 lColor = CalculateLigthing(frag);
	
	// -- Color from reflection and refraction -- 
//	if(u_reflectionFactor > 0.1) {
//		vec4 rflColor = CalculateReflection(frag);
//		vec4 rfrColor = CalculateRefraction(frag);
//	
//		float fresnelTerm = FresnelApproximation(dot(frag.frag_nvToCam, frag.frag_Normal), u_fresnelValues);
//
//		// -- Result --
//		frag_color = mix(tColor, mix(rfrColor, rflColor * lColor, fresnelTerm), u_reflectionFactor);
//	} else {
		frag_color = tColor * lColor;
//	}

	frag_color = clamp(frag_color, 0.0, 1.0);
//	frag_color = vec4(1.0, 0.0, 1.0, 1.0);
}
