#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_VPMat;
uniform mat4 u_Model;
uniform float u_Size;

out VS_OUT{
	mat4 MVPMat;
	float size;
} vs_out;

void main()
{
	gl_Position = u_VPMat * u_Model * vec4(aPos, 1.0); 
	vs_out.MVPMat = u_VPMat * u_Model;
	vs_out.size = u_Size;
}
