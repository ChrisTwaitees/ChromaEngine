#version 330 core
// out
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

// structs
struct Material
{
	//textures
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular1;
	sampler2D texture_specular2;

	samplerCube skybox;

	float roughness;
	float ambientBrightness;
	float specularIntensity;
};

struct DirLight 
{
	vec3 direction;
	vec3 diffuse;
	float intensity;
};

struct PointLight
{
	// attenuation
	float constant;
	float linear;
	float quadratic;
	// members
	vec3 position;
	vec3 diffuse;
	float intensity;
};

struct SpotLight
{
	// attenuation
	float constant;
	float linear;
	float quadratic;
	// members
	vec3 position;
	vec3 direction;
	float intensity;
	float spotSize;
	float penumbraSize;
	vec3 diffuse;
};

#define NR_POINT_LIGHTS 4
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 1

// in
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// uniforms
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

// functions prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseMap, vec3 specMap);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 diffuseMap, vec3 specMap);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos,  vec3 diffuseMap, vec3 specMap);


void main()
{
	vec3 result;
	// maps
	vec3 diffuseMap = vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specularMap = vec3(texture(material.texture_specular1, TexCoords));

	// attrs
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// direction lights
	for(int i = 0; i < NR_DIR_LIGHTS ; i++)
		result += CalcDirLight(dirLights[i], norm, viewDir,  diffuseMap, specularMap);

	
	// spot lights

	for(int i = 0; i < NR_SPOT_LIGHTS ; i++)
		//result += CalcSpotLight(spotLights[i], norm, viewDir, FragPos, diffuseMap, specularMap);


	// point lights
	for(int i = 0; i < NR_POINT_LIGHTS ; i++)
		result += CalcPointLight(pointLights[i], norm, viewDir, FragPos, diffuseMap, specularMap);

	FragColor = vec4(result, 1.0);

	//bloom
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}

#include "fragLightingCalculations.glsl"