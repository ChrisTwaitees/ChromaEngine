#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT{
	vec3 jointPos;
	vec3 childPos;
	mat4 VPMat;
	mat4 jointMat;
	float size;
} vs_out;

uniform mat4 VPMat;
uniform mat4 transform;
uniform vec3 JointPos;
uniform vec3 ChildPos;
uniform float Size;

void main()
{
	gl_Position = VPMat * transform * vec4(aPos, 1.0); 
	
	vs_out.jointPos = JointPos;
	vs_out.childPos = ChildPos;
	vs_out.size = Size;
	vs_out.VPMat = VPMat;
	vs_out.jointMat = transform;
}
