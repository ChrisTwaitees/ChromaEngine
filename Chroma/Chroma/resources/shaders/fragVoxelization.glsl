// Lit (diffuse) fragment voxelization shader.
// Author:	Fredrik Präntare <prantare@gmail.com> 
// Date:	11/26/2016
#version 450 core

in GS_OUT{
	vec3 worldPositionFrag;
	vec3 normalFrag;
	vec2 texCoordsFrag;
} gs_in;

// Lighting settings.
//#define POINT_LIGHT_INTENSITY 1
//#define MAX_LIGHTS 1

// Lighting attenuation factors.
//#define DIST_FACTOR 1.1f /* Distance is multiplied by this when calculating attenuation. */
//#define CONSTANT 1
//#define LINEAR 0
//#define QUADRATIC 1

// Returns an attenuation factor given a distance.
//float attenuate(float dist){ dist *= DIST_FACTOR; return 1.0f / (CONSTANT + LINEAR * dist + QUADRATIC * dist * dist); }
//
//struct PointLight {
//	vec3 position;
//	vec3 color;
//};
//
//struct Material {
//	vec3 diffuseColor;
//	vec3 specularColor;
//	float diffuseReflectivity;
//	float specularReflectivity;
//	float emissivity;
//	float transparency;
//};

//uniform Material material;
//uniform PointLight pointLights[MAX_LIGHTS];
//uniform int numberOfLights;
//uniform vec3 cameraPosition;
layout(RGBA8) uniform image3D texture3D;

//
//vec3 calculatePointLight(const PointLight light){
//	const vec3 direction = normalize(light.position - gs_in.worldPositionFrag);
//	const float distanceToLight = distance(light.position, gs_in.worldPositionFrag);
//	const float attenuation = attenuate(distanceToLight);
//	const float d = max(dot(normalize(gs_in.normalFrag), direction), 0.0f);
//	return d * POINT_LIGHT_INTENSITY * attenuation * light.color;
//};

vec3 scaleAndBias(vec3 p) { return 0.5f * p + vec3(0.5f); }

bool isInsideCube(const vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }

//void main(){
//	vec3 color = vec3(0.0f);
//	if(!isInsideCube(gs_in.worldPositionFrag, 0)) return;
//
//
//	// DEBUG
//	color = vec3(1.0f, 1.0f, 0.0f);
//	vec3 voxel = scaleAndBias(gs_in.worldPositionFrag);
//	ivec3 dim = imageSize(texture3D);
//	vec4 res =  vec4(vec3(color), 1);
//    imageStore(texture3D, ivec3(dim * voxel), res);
//	// DEBUG
//
//	// Calculate diffuse lighting fragment contribution.
////	const uint maxLights = min(numberOfLights, MAX_LIGHTS);
////	for(uint i = 0; i < maxLights; ++i) color += calculatePointLight(pointLights[i]);
////	vec3 spec = material.specularReflectivity * material.specularColor;
////	vec3 diff = material.diffuseReflectivity * material.diffuseColor;
////	color = (diff + spec) * color + clamp(material.emissivity, 0, 1) * material.diffuseColor;
//
//	// Output lighting to 3D texture.
////	vec3 voxel = scaleAndBias(gs_in.worldPositionFrag);
////	ivec3 dim = imageSize(texture3D);
////	float alpha = pow(1 - material.transparency, 4); // For soft shadows to work better with transparent materials.
////	vec4 res = alpha * vec4(vec3(color), 1);
////    imageStore(texture3D, ivec3(dim * voxel), res);
//
//}

// UNIFORMS
// Texture Checks
uniform bool UseAlbedoMap;
uniform bool UseNormalMap;
uniform bool UseMetRoughAOMap;

// MATERIALS
#include "util/materialStruct.glsl"
uniform Material material;
// Material overrides if no maps provided
uniform vec3 color;
uniform float roughness;
uniform float metalness;

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
uniform sampler2DArray shadowmap;

void main()
{
	// MATERIAL PROPERTIES
	vec3 Albedo, Normal;
	float Metalness, Roughness, AO;
	// albedo
	Albedo = UseAlbedoMap? vec3(texture(material.texture_albedo1, gs_in.texCoordsFrag)) : color;
	// normals
	if (UseNormalMap){
		Normal = vec3(texture(material.texture_normal1, gs_in.texCoordsFrag));
		Normal = normalize(Normal * 2.0 - 1.0);
		//Normal = normalize(fs_in.TBN * Normal);
	}
	else{
		Normal = gs_in.normalFrag;
	}
	// metalness roughness ao
	if(UseMetRoughAOMap){
		vec3 MetRoughAO = texture(material.texture_MetRoughAO1, gs_in.texCoordsFrag).rgb;
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
	vec3 viewDir = normalize(cameraPosition - gs_in.worldPositionFrag);
	// Directional Lights
//	for(int i = 0; i < numDirectionalLights ; i++)
//		Lo += CalcDirLight(UBO_DirLights[i], Normal, viewDir, Albedo, Roughness, Metalness, gs_in.worldPositionFrag, shadowmap);
//	// Point Lights
//	for(int i = 0; i < numPointLights ; i++)
//	{
//		if(length(UBO_PointLights[i].position - fs_in.FragPos) < UBO_PointLights[i].radius)
//			Lo += CalcPointLight(UBO_PointLights[i], Normal, viewDir, fs_in.FragPos, Albedo.rgb, Roughness, Metalness, gs_in.worldPositionFrag, shadowmap);
//	}
	// AMBIENT
	//------------------------------------------------------------------------
	// Adding ambient and SSAO
	vec3 Ambient = CalcAmbientLight(irradianceMap, prefilterMap, brdfLUT, Normal, viewDir, Albedo, Roughness, Metalness, AO, Lo.a);

	// COMBINE COLOR
	//------------------------------------------------------------------------
	vec3 color = (Ambient + Lo.rgb);

	// OUTPUT LIGHTING TO 3D TEXTURE
	//------------------------------------------------------------------------

	vec3 voxel = scaleAndBias(gs_in.worldPositionFrag);
	ivec3 dim = imageSize(texture3D);
//	float alpha = pow(1 - material.transparency, 4); // For soft shadows to work better with transparent materials.
	vec4 res = 1.0 * vec4(vec3(color), 1); //	vec4 res = alpha * vec4(vec3(color), 1);
    imageStore(texture3D, ivec3(dim * voxel), res);

}

#include "util/PBRLightingFuncsImplementation.glsl"