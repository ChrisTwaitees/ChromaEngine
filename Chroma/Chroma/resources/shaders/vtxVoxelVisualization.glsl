#version 450 core
layout(location = 0) in vec3 aPos;

// VOXELS
#include "util/voxelUniforms.glsl"
#include "util/voxelFuncs.glsl"

// Camera UBO
#include "util/uniformBufferCamera.glsl"

// Out
out VS_OUT{
	out mat4 VPMatGeom;
	out vec4 voxelColorGeom;
} vs_out;


void main(){
	ivec3 voxelSampleUVW = unflatten3D(gl_VertexID, u_VoxelGridResolution);
	vs_out.voxelColorGeom = texelFetch(u_VoxelTexture, voxelSampleUVW, 0);
	
	vs_out.VPMatGeom = projection * view;
	gl_Position =  vec4(voxelSampleUVW, 1.0);  
}