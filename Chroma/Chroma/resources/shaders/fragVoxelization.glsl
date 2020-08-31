// Lit (diffuse) fragment voxelization shader.
// Author:	Fredrik Präntare <prantare@gmail.com> 
// Date:	11/26/2016
#version 450 core

in GS_OUT{
//	vec4 voxelPositionFrag;
	vec3 worldPositionFrag;
	vec3 normalFrag;
	vec2 texCoordsFrag;
} gs_in;
uniform int voxelResolution;
uniform vec3 voxelGridCentroid;


//uniform vec3 cameraPosition;
layout(RGBA8) uniform image3D texture3D;

vec3 scaleAndBias(vec3 p) { return p * vec3(0.5f, -0.5f, 0.5f) + vec3(0.5f); }

bool isInsideCube(const vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }


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
	// COLLECT VOXEL DATA
	// --------------------------------------
	// World Position -> Voxel Grid Space -> Clip Space
	vec3 voxelUVW = (gs_in.worldPositionFrag - voxelGridCentroid) * ( 1.0 / voxelResolution);
	// Clip Space -1 : 1 -> Clip Space 0 : 1 
	voxelUVW = scaleAndBias(voxelUVW);
	// Check within 3D Texture bounds
	if(!isInsideCube(voxelUVW, 0.0))
		return;
	// --------------------------------------


	// LIGHTING
	// --------------------------------------
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

	// DIRECT LIGHTING
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
	// COMBINE COLOR
	//------------------------------------------------------------------------
	//vec3 color = (Lo.rgb);
	vec3 color = Albedo.rgb;

	// OUTPUT TO 3D TEXTURE
	//------------------------------------------------------------------------
	vec4 writeData =  vec4(color, 1); //	vec4 res = alpha * vec4(vec3(color), 1);
	// Clip Space -> Voxel Grid Space
	ivec3 writeCoord = ivec3(floor(voxelUVW * voxelResolution));
    imageStore(texture3D, writeCoord, writeData);
}

#include "util/PBRLightingFuncsImplementation.glsl"