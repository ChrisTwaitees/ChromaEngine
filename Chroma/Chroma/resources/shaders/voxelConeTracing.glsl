#version 450 core
out vec4 FragColor;
in vec2 TexCoords;

// VOXEL
#include "util/voxelUniforms.glsl"
#include "util/voxelFuncs.glsl"

// textures
uniform sampler2D u_PositionsWS;
uniform sampler2D u_NormalsWS;
uniform sampler2D u_MetRoughAO;

// Camera
#include "util/uniformBufferCamera.glsl"

void main()
{	
	vec3 PosWS = texture(u_PositionsWS, TexCoords.st).xyz;
	vec3 NormWS = normalize(texture(u_NormalsWS, TexCoords.st).xyz);
	vec4 MetRoughAO = texture(u_MetRoughAO, TexCoords.st);

	// Radiance
	vec4 radiance = ConeTraceRadiance(u_VoxelTexture, PosWS, NormWS);
	// Reflection
	vec3 viewDir = (cameraPosition - PosWS);
	vec4 reflection = ConeTraceReflection(u_VoxelTexture, PosWS, NormWS, viewDir, MetRoughAO.g);

	// OUT
	FragColor = radiance;
	
}  