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
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace, float SSAO, sampler2D shadowmap);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace, float SSAO, sampler2D shadowmap);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos,  vec3 diffuseMap, vec3 specMap);
float ShadowCalculation(vec4 FragPosLightSpace, sampler2D shadowmap, vec3 normal, vec3 lightDir);

void main()
{
	vec3 result;
	// maps
	vec3 albedoMap = vec3(texture(material.texture_albedo1, fs_in.TexCoords));
	float metalnessMap = texture(material.texture_metalness1, fs_in.TexCoords).r;
	float roughnessMap = texture(material.texture_roughness1, fs_in.TexCoords).r;
	float aoMap = texture(material.texture_ao1, fs_in.TexCoords).r;

	// normals
	vec3 normals;
	if (UseNormalMap && length(fs_in.TBN[1]) >= 0.5 ){
		normals = vec3(texture(material.texture_normal1, fs_in.TexCoords));
		normals = normalize(normals * 2.0 - 1.0);
		normals = normalize(fs_in.TBN * normals);
	}
	else{
		normals = fs_in.Normal;
	}

	// TODO : swap out hard coded values
	float ambient = 0.3;
	float SSAO = 0.0;

	// attrs
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// direction lights
	for(int i = 0; i < NR_DIR_LIGHTS ; i++)
		result += CalcDirLight(dirLights[i], normals, viewDir, albedoMap, roughnessMap, metalnessMap, metalnessMap, ambient, fs_in.FragPosLightSpace, aoMap, shadowmaps.shadowmap1);
	
	// spot lights
	//for(int i = 0; i < NR_SPOT_LIGHTS ; i++)
		//result += CalcSpotLight(spotLights[i], norm, viewDir, FragPos, diffuseMap, specularMap, normalMap);

	// point lights
	for(int i = 0; i < NR_POINT_LIGHTS ; i++)
		result += CalcPointLight(pointLights[i], normals, viewDir, fs_in.FragPos, albedoMap, roughnessMap, metalnessMap, metalnessMap, ambient, fs_in.FragPosLightSpace, aoMap, shadowmaps.shadowmap1);

	FragColor = vec4(result, 1.0);
	// bloom
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

#include "util/lightingCalculations.glsl"