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
#include "util/lightingStructs.glsl"
#define NR_POINT_LIGHTS 3
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 1

// SHADOWMAPS
struct ShadowMap
{
	sampler2D shadowmap1;
};

// UNIFORMS

// MATERIALS
#include "util/materialStruct.glsl"
uniform Material material;

// Texture Checks
uniform bool UseAlbedoMap;
uniform bool UseNormalMap;
uniform bool UseMetRoughAOMap;
uniform bool UseTranslucencyMap;
// Material overrides if no maps provided
uniform vec3 color;
uniform float roughness;
uniform float metalness;
uniform vec2 UVMultiply;

uniform ShadowMap shadowmaps;

// UNIFORMS
// backscatter
//uniform float bckScttrScale;
//uniform float bckScttrPow;
//uniform float bckScttrDistortion;

uniform vec3 viewPos;
//IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;

// Lights
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

// Lighting Functions
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metalness, vec4 FragPosLightSpace, sampler2D shadowmap);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo, float roughness, float metalness, vec4 FragPosLightSpace, sampler2D shadowmap);
vec3 CalcAmbientLight(samplerCube irradianceMap, samplerCube prefilterMap, sampler2D brdfLUT, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metalness, float ao, float shadows);

void main()
{
	// MATERIAL PROPERTIES
	vec3 Albedo, Normal;
	float Alpha, Metalness, Roughness, AO, Translucency;
	// albedo
	if(UseAlbedoMap)
	{
		vec4 texMap = texture(material.texture_albedo1, fs_in.TexCoords * UVMultiply);
		Albedo = texMap.rgb;
		Alpha = texMap.w;
	}
	else{
		Albedo = color;
		Alpha = 1.0;
	}

	// translucency
	Translucency = UseTranslucencyMap ? texture(material.texture_translucency1, fs_in.TexCoords * UVMultiply).r : 0.0;

	// normals
	if (UseNormalMap){
		Normal = vec3(texture(material.texture_normal1, fs_in.TexCoords * UVMultiply));
		Normal = normalize(Normal * 2.0 - 1.0);
		Normal = normalize(fs_in.TBN * Normal);
	}
	else{
		Normal = fs_in.Normal;
	}
	// metalness roughness ao
	if(UseMetRoughAOMap){
		vec3 MetRoughAO = texture(material.texture_MetRoughAO1, fs_in.TexCoords * UVMultiply).rgb;
		Metalness = MetRoughAO.r ;
		Roughness = MetRoughAO.g;
		AO = MetRoughAO.b;
	}
	else{
		Metalness = metalness;
		Roughness = roughness;
		AO = 1.0;
	}
	
	// TODO : fetch SSAO from GBuffer
	float SSAO = 1.0;

	// LIGHTING
	//------------------------------------------------------------------------
	// PBR calculates irradiance, denoted by Lo
	vec4 Lo;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	// Directional Lights
	for(int i = 0; i < NR_DIR_LIGHTS ; i++)
		Lo += CalcDirLight(dirLights[i], Normal, viewDir, Albedo, Roughness, Metalness, fs_in.FragPosLightSpace, shadowmaps.shadowmap1);
	// Point Lights
	for(int i = 0; i < NR_POINT_LIGHTS ; i++)
		Lo += CalcPointLight(pointLights[i], Normal, viewDir, fs_in.FragPos, Albedo, Roughness, Metalness, fs_in.FragPosLightSpace, shadowmaps.shadowmap1);

	// BACK SCATTER
	float bckScttrAmount;
	float bckScttrScale      = 1.1;
	float bckScttrPow        = 0.695;
	float bckScttrDistortion = 0.695;

	// Directional Lights
	for(int i = 0; i < NR_DIR_LIGHTS ; i++)
	{
		vec3 HlfWaySSDistortion = normalize(-dirLights[i].direction + Normal * bckScttrDistortion );
		bckScttrAmount += pow(clamp(dot(viewDir, -HlfWaySSDistortion), 0.0, 1.0), bckScttrPow ) * bckScttrScale * Translucency ; 
		Lo += vec4(dirLights[i].diffuse.rgb, 1.0) * vec4(Albedo, 1.0) * bckScttrAmount;
	}
		

	// AMBIENT
	//------------------------------------------------------------------------
	// Adding ambient and SSAO
	vec3 Ambient = CalcAmbientLight(irradianceMap, prefilterMap, brdfLUT, Normal, viewDir, Albedo, Roughness, Metalness, AO, Lo.a) * SSAO;

	// COMBINE
	//------------------------------------------------------------------------
	vec3 color = (Ambient + Lo.rgb) * SSAO;

	// OUT
	//------------------------------------------------------------------------
	if(Alpha > 0.1)
	{
		FragColor = vec4(vec3(color), Alpha);
	}
	else
		discard;

	// POST FX
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

#include "util/PBRLightingCalculations.glsl"