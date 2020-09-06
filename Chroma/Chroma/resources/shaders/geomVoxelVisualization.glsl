#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

// UNIFORMS
// VOXELS
#include "util/voxelUniforms.glsl"
// Encode
#include "util/encodeData.glsl"

in VS_OUT{
	mat4 VPMatGeom;
	vec4 voxelColorGeom;
} vs_in[];

out GS_OUT{
	vec4 voxelColorFrag;
	vec3 voxelNormalFrag;
} gs_out;

void createVertex(vec3 inPos)
{
	vec4 worldPos = gl_in[0].gl_Position;
	
	// Shifting grid to world center
	worldPos.xyz = (worldPos.xyz - vec3(u_VoxelGridResolution) * vec3(0.5)) * 2.0; 	

	// Remap Voxel Grid Space -> World Space
	//1. Voxel Grid Space -> Clip Space
	worldPos.xyz /= vec3(u_VoxelGridResolution);
	// 2. Clip Space (0:1) -> Clip Space (-1:1)
	worldPos.xyz *= vec3(2.0f) - vec3(1.0f);
	// 3. Clip Space (-1:1) -> World Space
	worldPos.xyz *= vec3(u_VoxelGridResolution);
	
	// ?? Factoring Grid Resolution and Size
	worldPos.xyz *= u_VoxelGridResolution * u_VoxelGridSize / u_VoxelGridResolution;
	
	// Move to voxel centroid
	worldPos.xyz += u_VoxelGridCentroid;

	// Create Cube
	worldPos.xyz += inPos * u_VoxelGridSize;

	// VPMat
	gl_Position = vs_in[0].VPMatGeom * worldPos;
	
	EmitVertex();
}


void main()
{

	vec4 voxelColor = vs_in[0].voxelColorGeom;
	if(voxelColor.a == 0.0)
		return;

	gs_out.voxelColorFrag = vs_in[0].voxelColorGeom;
	gs_out.voxelNormalFrag =  UnPackNormal(vs_in[0].voxelColorGeom.a);
	
	createVertex(vec3(-1.0, 1.0, 1.0));
	createVertex(vec3(-1.0, -1.0, 1.0));
	createVertex(vec3(1.0, 1.0, 1.0));
	createVertex(vec3(1.0, -1.0, 1.0));
	
	EndPrimitive();
	
	createVertex(vec3(1.0, 1.0, 1.0));
	createVertex(vec3(1.0, -1.0, 1.0));
	createVertex(vec3(1.0, 1.0, -1.0));
	createVertex(vec3(1.0, -1.0, -1.0));
	
	EndPrimitive();
	
	createVertex(vec3(1.0, 1.0, -1.0));
	createVertex(vec3(1.0, -1.0, -1.0));
	createVertex(vec3(-1.0, 1.0, -1.0));
	createVertex(vec3(-1.0, -1.0, -1.0));
	
	EndPrimitive();
	
	createVertex(vec3(-1.0, 1.0, -1.0));
	createVertex(vec3(-1.0, -1.0, -1.0));
	createVertex(vec3(-1.0, 1.0, 1.0));
	createVertex(vec3(-1.0, -1.0, 1.0));
	
	EndPrimitive();
	
	createVertex(vec3(1.0, 1.0, 1.0));
	createVertex(vec3(1.0, 1.0, -1.0));
	createVertex(vec3(-1.0, 1.0, 1.0));
	createVertex(vec3(-1.0, 1.0, -1.0));
	
	EndPrimitive();
	
	createVertex(vec3(-1.0, -1.0, 1.0));
	createVertex(vec3(-1.0, -1.0, -1.0));
	createVertex(vec3(1.0, -1.0, 1.0));
	createVertex(vec3(1.0, -1.0, -1.0));
	
	EndPrimitive();



}