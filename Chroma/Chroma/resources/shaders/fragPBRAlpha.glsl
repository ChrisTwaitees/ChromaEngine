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

// LIGHTS
#define NR_POINT_LIGHTS 4
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 1
#include "util/lightingStructs.glsl"

// MATERIALS
#include "util/materialStruct.glsl"

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
uniform bool UseNormalMap;

// Lighting
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metalness, vec4 FragPosLightSpace, sampler2D shadowmap);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo, float roughness, float metalness, vec4 FragPosLightSpace, sampler2D shadowmap);

void main()
{
	// TEXTURE MAPS
	vec4 albedoMap = texture(material.texture_albedo1, fs_in.TexCoords);
	vec3 albedo = albedoMap.rgb;
	float alpha = albedoMap.a;
	// normals
	vec3 normal;
	if (UseNormalMap && length(fs_in.TBN[1]) >= 0.5 ){
		normal = vec3(texture(material.texture_normal1, fs_in.TexCoords));
		normal = normalize(normal * 2.0 - 1.0);
		normal = normalize(fs_in.TBN * normal);
	}
	else{
		normal = fs_in.Normal;
	}
	// metalness roughness ao
	vec3 MetRoughAO = texture(material.texture_MetRoughAO1, fs_in.TexCoords).rgb;
	float metalness = MetRoughAO.r ;
	float roughness = MetRoughAO.g;
	float ao = MetRoughAO.b;

	// VARS
	// TODO : swap out hard coded values
	float SSAO = 1.0;

	// LIGHTING
	//------------------------------------------------------------------------
	// PBR calculates irradiance, denoted by Lo
	vec3 Lo;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	// Directional Lights
	for(int i = 0; i < NR_DIR_LIGHTS ; i++)
		Lo += CalcDirLight(dirLights[i], normal, viewDir, albedo, roughness, metalness, fs_in.FragPosLightSpace, shadowmaps.shadowmap1);
	// Point Lights
	for(int i = 0; i < NR_POINT_LIGHTS ; i++)
		Lo += CalcPointLight(pointLights[i], normal, viewDir, fs_in.FragPos, albedo, roughness, metalness, fs_in.FragPosLightSpace, shadowmaps.shadowmap1);
	// Adding ambient and SSAO
	vec3 ambient = vec3(0.03) * albedo * ao * SSAO;
	// Creating color 
	vec3 color = ambient + Lo;
	//------------------------------------------------------------------------
	// OUT
	FragColor = vec4(color, alpha);

	// POST FX
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

#include "util/PBRLightingCalculations.glsl"