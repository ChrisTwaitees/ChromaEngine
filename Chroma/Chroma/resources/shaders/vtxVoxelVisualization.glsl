#version 450 core

layout(location = 0) in vec3 aPos;

// Camera UBO
#include "util/uniformBufferCamera.glsl"

// Uniforms
uniform sampler3D voxelTexture;

uniform int voxelGridResolution;
uniform vec3 voxelGridCentroid;
uniform float voxelGridSize;

// Out
out VS_OUT{
	out mat4 VPMat;
	out vec4 voxelColorGeom;
	out float boxSize;
} vs_out;

// Map index to resolution * resolution * resolution 
// Unpacking Texels of 3DTexture
ivec3 unflatten3D(int idx, int resolution)
{
	ivec3 dim = ivec3(resolution);
	const int z = idx / (dim.x * dim.y);
	idx -= (z * dim.x * dim.y);
	const int y = idx / dim.x;
	const int x = idx % dim.x;

	return ivec3(x,y,z);
}

void main(){
	ivec3 voxelSampleUVW = unflatten3D(gl_VertexID, voxelGridResolution);
	vs_out.voxelColorGeom = texelFetch(voxelTexture, voxelSampleUVW, 0);
	
	vs_out.VPMat = projection * view;
	vs_out.boxSize =  1.0/voxelGridResolution * 1 / voxelGridSize * 0.5;//(1 / pow(voxelGridSize,3)) * voxelGridSize*10000;
	gl_Position =  vec4(voxelSampleUVW, 1.0);  
}