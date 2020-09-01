#version 450 core

layout(location = 0) in vec3 aPos;
#include "util/uniformBufferCamera.glsl"
uniform sampler3D voxelTexture; // Texture in which voxelization is stored.
uniform int voxelRes;


out VS_OUT{
	out mat4 VPMat;
	out vec4 voxelColorGeom;
	out float boxSize;
} vs_out;

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

	ivec3 voxelSampleUVW = unflatten3D(gl_VertexID, voxelRes);
	vs_out.voxelColorGeom = texelFetch(voxelTexture, voxelSampleUVW, 0);
	//vs_out.voxelColorGeom = vec4(1,1,0,1);
	

	vs_out.VPMat = projection * view;
	vs_out.boxSize = 0.32;
	gl_Position =  vec4(voxelSampleUVW, 1.0);  
	//gl_Position = vec4(0,0,0,1);
}