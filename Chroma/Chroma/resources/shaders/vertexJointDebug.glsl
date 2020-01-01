#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT{
	vec3 originPos;
	vec3 childPos;
	mat4 MVPMat;
	float size;
} vs_out;

uniform mat4 VPMat;
uniform mat4 transform;
uniform vec3 OriginPos;
uniform vec3 ChildPos;
uniform float Size;

void main()
{
	gl_Position = VPMat * transform * vec4(aPos, 1.0); 
	vs_out.originPos = OriginPos;
	vs_out.childPos = ChildPos;
	vs_out.size = Size;
	vs_out.MVPMat = VPMat * transform;
}
