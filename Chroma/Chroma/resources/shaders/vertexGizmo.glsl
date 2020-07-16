#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT{
	mat4 MVPMat;
	float size;
} vs_out;

uniform mat4 VPMat;
uniform mat4 model;
uniform float scale;

void main()
{
	gl_Position = VPMat * model * vec4(aPos, 1.0); 
	vs_out.MVPMat = VPMat * model;
	vs_out.size = scale;
}
