#version 450 core

layout (location = 0) out vec4 FragColor;

in GS_OUT{
	vec4 voxelColorFrag;
	vec3 voxelNormalFrag;
} gs_in;


void main()
{
	FragColor = gs_in.voxelColorFrag;
	//FragColor = vec4(normalize(gs_in.voxelNormalFrag), 1.0);
}