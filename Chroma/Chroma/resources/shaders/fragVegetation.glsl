#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
	mat3 TBN;
	vec4 Color;
} fs_in;


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
// SHADOWMAPS
uniform sampler2DArray shadowmap;

// UNIFORMS
// BACK SCATTER
float bckScttrAmount;
const float bckScttrScale      = 0.8;
const float bckScttrPow        = 0.695;
const float bckScttrDistortion = 0.695;

//IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;

// LIGHTING
#include "util/lightingStructs.glsl"
// ubos
#include "util/uniformBufferLighting.glsl"
#include "util/uniformBufferCamera.glsl"
// Lighting Functions
#include "util/PBRLightingFuncsDeclaration.glsl"

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
	vec3 viewDir = normalize(cameraPosition - fs_in.FragPos);
	// Directional Lights
	for(int i = 0; i < numDirectionalLights ; i++)
		Lo += CalcDirLight(UBO_DirLights[i], Normal, viewDir, Albedo, Roughness, Metalness, fs_in.FragPosLightSpace, shadowmap);
	// Point Lights
	for(int i = 0; i < numPointLights ; i++)
	{
		if(length(UBO_PointLights[i].position - fs_in.FragPos) < UBO_PointLights[i].radius)
			Lo += CalcPointLight(UBO_PointLights[i], Normal, viewDir, fs_in.FragPos, Albedo.rgb, Roughness, Metalness, fs_in.FragPosLightSpace, shadowmap);
	}
	// TRANSLUCENCY
	// Directional Lights
	if(UseTranslucencyMap)
	{
		for(int i = 0; i < numDirectionalLights ; i++)
		{
			vec3 HlfWaySSDistortion = normalize(-UBO_DirLights[i].direction + Normal * bckScttrDistortion );
			bckScttrAmount += pow(clamp(dot(viewDir, -HlfWaySSDistortion), 0.0, 1.0), bckScttrPow ) * bckScttrScale * Translucency ; 
			Lo += vec4(UBO_DirLights[i].diffuse.rgb, 1.0) * vec4(Albedo, 1.0) * bckScttrAmount;
		}
	}
	
	// AMBIENT
	//------------------------------------------------------------------------
	// Adding ambient and SSAO
	vec3 Ambient = CalcAmbientLight(irradianceMap, prefilterMap, brdfLUT, Normal, viewDir, Albedo, Roughness, Metalness, AO, Lo.a);

	// COMBINE
	//------------------------------------------------------------------------
	vec3 color = (Ambient + Lo.rgb) * SSAO;

	// OUT
	//------------------------------------------------------------------------
	if(Alpha > 0.65)
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

#include "util/PBRLightingFuncsImplementation.glsl"