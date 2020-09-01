#version 450 core

layout (location = 0) out vec4 FragColor;

in GS_OUT{
	vec4 voxelColorFrag;
} gs_in;


void main()
{

	FragColor = gs_in.voxelColorFrag;
}