#version 330 core
out VS_OUT{
	mat4 MVPMat;
	float size;
} vs_out;

uniform mat4 VPMat;
uniform mat4 model;
uniform float Size;

void main()
{
	vs_out.size = Size;
	vs_out.MVPMat = VPMat * model;
}
