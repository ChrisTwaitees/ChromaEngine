#version 450 core

in GS_OUT{
	vec3 worldPositionFrag;
	vec3 normalFrag;
	vec2 texCoordsFrag;
	vec4 posLightSpaceFrag;
} gs_in;

// UNIFORMS
layout(RGBA8) uniform image3D texture3D;
uniform sampler2DArray shadowmap;

// Voxel Uniforms
uniform int voxelGridResolution;
uniform vec3 voxelGridCentroid;
uniform float voxelGridSize;

// UNIFORMS
// Texture Checks
uniform bool UseAlbedoMap;

// LIGHTING
#include "util/lightingStructs.glsl"
// ubos
#include "util/uniformBufferLighting.glsl"
#include "util/uniformBufferCamera.glsl"
// Lighting Functions
#include "util/lightingDirectFuncsDeclaration.glsl"

// MATERIALS
#include "util/materialStruct.glsl"
uniform Material material;
// Material overrides if no maps provided
uniform vec3 color;

// Remap from -1 : 1 to 0 : 1
vec3 remap1101(vec3 p) { return p * vec3(0.5f, 0.5f, 0.5f) + vec3(0.5f); }
// Check whether coordinate is in unit cube
bool inVoxelGrid(const vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }


void main()
{
	// REMAP VOXEL COORDS
	// --------------------------------------
	// World Position -> Voxel Grid Space
	vec3 voxelUVW = (gs_in.worldPositionFrag - voxelGridCentroid) * (1.0 / voxelGridResolution) * (1.0 / voxelGridSize) ;
	// Voxel Grid Space -> Clip Space (-1 : 1) -> Clip Space (0 : 1) 
	voxelUVW = remap1101(voxelUVW);

	// Check within 3D Texture bounds
	if(inVoxelGrid(voxelUVW, 0.0))
	{
		// --------------------------------------
		// LIGHTING
		// --------------------------------------
		// Albedo
		vec3 Albedo = UseAlbedoMap? vec3(texture(material.texture_albedo1, gs_in.texCoordsFrag)) : color;
		// Normals
		vec3 Normal = normalize(gs_in.normalFrag);
		// worldPosition
		vec4 worldPos = vec4(gs_in.worldPositionFrag,1.0);
		// view direction
		vec3 viewDir = normalize(cameraPosition - worldPos.xyz);

		// DIRECT LIGHTING
		//------------------------------------------------------------------------
		vec4 radiance;

		// Directional Lights
//		for (uint i = 0 ; i < numDirectionalLights ; i++)
//		{
//			radiance += CalcDirLight( UBO_DirLights[i], Normal, viewDir, Albedo, gs_in.posLightSpaceFrag, shadowmap);
//		}
		// Point Lights
		for (uint i = 0 ; i < numPointLights ; ++i)
		{
			if (length(UBO_PointLights[i].position - worldPos.xyz) < UBO_PointLights[i].radius)
				radiance += CalcPointLight(UBO_PointLights[i], Normal,  viewDir, worldPos.xyz, Albedo);
		}
		// Spot Lights
		for (uint i = 0 ; i < numSpotLights ; ++i)
		{
			radiance += CalcSpotLight(UBO_SpotLights[i], Normal,  viewDir, worldPos.xyz, Albedo);
		}

		// PREPARE OUTPUT
		//------------------------------------------------------------------------
		// if no direct lighting is detected, write 0.0 alpha
		float alpha = radiance.rgb == vec3(0.0) ? 0.0 : 1.0;
		//float alpha = 1.0;

		// OUTPUT TO 3D TEXTURE
		//------------------------------------------------------------------------
		vec4 writeData =  vec4(radiance.rgb, alpha);
		// Clip Space -> Voxel Grid Space
		ivec3 writeCoord = ivec3(floor(voxelUVW * voxelGridResolution));
		imageStore(texture3D, writeCoord, writeData);
	}
	else
		return;
}

// Function Implementations
#include "util/lightingDirectFuncsImplementation.glsl"