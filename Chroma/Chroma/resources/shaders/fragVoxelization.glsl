#version 450 core

in GS_OUT{
	vec3 worldPositionFrag;
	vec3 normalFrag;
	vec2 texCoordsFrag;
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
// MATERIALS
#include "util/materialStruct.glsl"
uniform Material material;
// Material overrides if no maps provided
uniform vec3 color;

vec3 remap1101(vec3 p) { return p * vec3(0.5f, 0.5f, 0.5f) + vec3(0.5f); }
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
	if(!inVoxelGrid(voxelUVW, 0.0))
		return;
	// --------------------------------------

	// LIGHTING
	// --------------------------------------
	// MATERIAL PROPERTIES
	vec3 Albedo, Normal;
	// Albedo
	Albedo = UseAlbedoMap? vec3(texture(material.texture_albedo1, gs_in.texCoordsFrag)) : color;
	// Normals
	Normal = gs_in.normalFrag;

	// DIRECT LIGHTING
	//------------------------------------------------------------------------

	// POINT LIGHTING
	//------------------------------------------------------------------------

	// SPOT LIGHTING
	//------------------------------------------------------------------------


	//vec3 color = (Lo.rgb);
	vec3 color = Albedo.rgb;

	// OUTPUT TO 3D TEXTURE
	//------------------------------------------------------------------------
	vec4 writeData =  vec4(color, 1); //	vec4 res = alpha * vec4(vec3(color), 1);
	// Clip Space -> Voxel Grid Space
	ivec3 writeCoord = ivec3(floor(voxelUVW * voxelGridResolution));
    imageStore(texture3D, writeCoord, writeData);
}

#include "util/PBRLightingFuncsImplementation.glsl"