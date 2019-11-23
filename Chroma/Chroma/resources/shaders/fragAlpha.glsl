#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
	mat3 TBN;
} fs_in;

// MAX_LIGHTS
#define NR_POINT_LIGHTS 4
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 1

// MATERIALS
struct Material
{
	//textures
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;

	//cubemap
	float cubemapIntensity;
	samplerCube skybox;

	// refraction
	float refractionIntensity;

	float roughness;
	float ambientBrightness;
	float specularIntensity;
};

#include "fragLightingStructs.glsl"

// SHADOWMAPS
struct ShadowMap
{
	sampler2D shadowmap1;
};

// UNIFORMS
uniform vec3 viewPos;
uniform Material material;
uniform ShadowMap shadowmaps;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

// Lighting
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseMap, vec3 specMap);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 diffuseMap, vec3 specMap);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos,  vec3 diffuseMap, vec3 specMap);
float ShadowCalculation(vec4 FragPosLightSpace, vec3 normal, vec3 lightDir);

void main()
{

	vec3 result;
	// maps
	vec4 diffuseMap = vec4(texture(material.texture_diffuse1, fs_in.TexCoords));
	vec3 specularMap = vec3(texture(material.texture_specular1, fs_in.TexCoords));
	vec3 normalMap = vec3(texture(material.texture_normal1, fs_in.TexCoords));

	// attrs
	vec3 norm = normalize(fs_in.Normal);
	// if normal map used, transform from tangent to world space
	//vec3 norm = normalize(normalMap * 2.0 - 1.0);
	norm = normalize(fs_in.TBN * norm);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// direction lights
	for(int i = 0; i < NR_DIR_LIGHTS ; i++)
		result += CalcDirLight(dirLights[i], norm, viewDir,  diffuseMap.rgb, specularMap);
	
	// spot lights
	//for(int i = 0; i < NR_SPOT_LIGHTS ; i++)
		//result += CalcSpotLight(spotLights[i], norm, viewDir, FragPos, diffuseMap, specularMap, normalMap);

	// point lights
	for(int i = 0; i < NR_POINT_LIGHTS ; i++)
		result += CalcPointLight(pointLights[i], norm, viewDir, fs_in.FragPos, diffuseMap.rgb, specularMap);

	if(diffuseMap.a < 0.1)
	{
		discard;
	}
	else{
		FragColor = vec4(result, diffuseMap.a);
	}


	// bloom
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

#include "fragLightingCalculations.glsl"