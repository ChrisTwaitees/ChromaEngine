#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT{
	vec3 bbox_min;
	vec3 bbox_max;
	mat4 MVPMat;
} vs_out;

uniform mat4 VPMat;
uniform mat4 model;
uniform vec3 BBoxMin;
uniform vec3 BBoxMax;

void main()
{
	gl_Position = VPMat * model * vec4(aPos, 1.0); 
	vs_out.bbox_min = BBoxMin;
	vs_out.bbox_max = BBoxMax;
	vs_out.MVPMat = VPMat * model;
}
