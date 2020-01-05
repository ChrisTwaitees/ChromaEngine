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
#define NR_POINT_LIGHTS 1
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 1

// SHADOWMAPS
struct ShadowMap
{
	sampler2D shadowmap1;
};

// UNIFORMS
// Texture Checks
uniform bool UseAlbedoMap;
uniform bool UseNormalMap;
uniform bool UseMetRoughAOMap;
// Material overrides if no maps provided
uniform vec4 color;
uniform float roughness;
uniform float metalness;


// MATERIALS
#include "util/materialStruct.glsl"
uniform Material material;

uniform ShadowMap shadowmaps;

// UNIFORMS
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
	vec4 Albedo;
	// MATERIAL PROPERTIES
	vec3 Normal;
	float Metalness, Roughness, AO;
	// albedo
	Albedo = UseAlbedoMap? vec4(texture(material.texture_albedo1, fs_in.TexCoords)) : color;
	float Alpha = Albedo.a;
	// normals
	if (UseNormalMap && length(fs_in.TBN[1]) >= 0.5 ){
		Normal = vec3(texture(material.texture_normal1, fs_in.TexCoords));
		Normal = normalize(Normal * 2.0 - 1.0);
		Normal = normalize(fs_in.TBN * Normal);
	}
	else{
		Normal = fs_in.Normal;
	}
	// metalness roughness ao
	if(UseMetRoughAOMap){
		vec3 MetRoughAO = texture(material.texture_MetRoughAO1, fs_in.TexCoords).rgb;
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
	float SSAO = 0.1;

	// LIGHTING
	//------------------------------------------------------------------------
	// PBR calculates irradiance, denoted by Lo
	vec4 Lo;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	// Directional Lights
	for(int i = 0; i < NR_DIR_LIGHTS ; i++)
		Lo += CalcDirLight(dirLights[i], Normal, viewDir, Albedo.rgb, Roughness, Metalness, fs_in.FragPosLightSpace, shadowmaps.shadowmap1);
	// Point Lights
	for(int i = 0; i < NR_POINT_LIGHTS ; i++)
		Lo += CalcPointLight(pointLights[i], Normal, viewDir, fs_in.FragPos, Albedo.rgb, Roughness, Metalness, fs_in.FragPosLightSpace, shadowmaps.shadowmap1);

	// AMBIENT
	//------------------------------------------------------------------------
	// Adding ambient and SSAO
	vec3 Ambient = CalcAmbientLight(irradianceMap, prefilterMap, brdfLUT, Normal, viewDir, Albedo.rgb, Roughness, Metalness, AO, Lo.a) * SSAO;

	// COMBINE
	//------------------------------------------------------------------------
	vec3 color = (Ambient + Lo.rgb) * SSAO;

	// OUT
	//------------------------------------------------------------------------
	//float alpha = 0.5;
	if(fs_in.TexCoords.x < 0.5)
	{
		FragColor = vec4(vec3(fs_in.TexCoords.xyx), 1.0 - fs_in.TexCoords.x);
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