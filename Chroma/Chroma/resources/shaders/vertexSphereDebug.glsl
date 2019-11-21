#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT{
	float radius;
} vs_out;

uniform mat4 VPMat;
uniform mat4 model;
uniform float radius;


void main()
{
 	gl_Position = VPMat * model * vec4(aPos, 1.0); 
	vs_out.radius = radius;
}