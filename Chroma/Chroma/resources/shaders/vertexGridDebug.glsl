#version 330 core
out VS_OUT{
	mat4 MVPMat;
	float size;
} vs_out;

uniform mat4 VPMat;
uniform mat4 model;
uniform float size;

void main()
{
	vs_out.size = size;
	vs_out.MVPMat = model * VPMat;
}
