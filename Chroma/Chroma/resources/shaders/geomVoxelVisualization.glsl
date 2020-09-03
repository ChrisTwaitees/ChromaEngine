#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

// Voxel Uniforms
uniform int voxelGridResolution;
uniform vec3 voxelGridCentroid;
uniform float voxelGridSize;

// UNIFORMS

in VS_OUT{
	mat4 VPMat;
	vec4 voxelColorGeom;
	float boxSize;
} vs_in[];

out GS_OUT{
	vec4 voxelColorFrag;
} gs_out;

void createVertex(vec3 inPos)
{
	vec4 worldPos = gl_in[0].gl_Position;
//	worldPos.x -= voxelGridResolution * voxelGridSize * 0.5; 
//	worldPos.z -= voxelGridResolution * voxelGridSize * 0.5; 
//	worldPos *= (1.0 / voxelGridSize);

	// Remap Voxel Grid Space -> World Space
	// 1. Voxel Grid Space -> Clip Space
	worldPos.xyz /= vec3(voxelGridResolution);
	// 2. Clip Space (0:1) -> Clip Space (-1:1)
	worldPos.xyz *= vec3(2.0f) - vec3(1.0f);
	// 3. Clip Space (-1:1) -> World Space
	worldPos.xyz *= vec3(voxelGridResolution);
	
	// ?? Factoring Grid Resolution and Size
	worldPos.xyz *= voxelGridResolution * voxelGridSize / voxelGridResolution;
	
	// Move to voxel centroid
//	worldPos.xyz += voxelGridCentroid;

	// Add Cube
	worldPos.xyz += inPos * vs_in[0].boxSize;

	// VPMat
	gl_Position = vs_in[0].VPMat * worldPos;
	
	EmitVertex();
}


void main()
{

	vec4 voxelColor = vs_in[0].voxelColorGeom;
	if(voxelColor.rgb == vec3(0.0))
		return;

	gs_out.voxelColorFrag = vs_in[0].voxelColorGeom;
	
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