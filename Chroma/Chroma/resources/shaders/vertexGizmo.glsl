#version 330 core
layout (location = 0) in vec3 aPos;


// UNIFORMS
#include "util/uniformBufferCamera.glsl"

uniform mat4 u_Model;
uniform float u_Size;

out VS_OUT{
	mat4 MVPMat;
	int vertexID;
	float size;
} vs_out;

void main()
{
	gl_Position = projection * view * u_Model * vec4(aPos, 1.0); 
	vs_out.MVPMat = projection * view * u_Model;
	vs_out.size = u_Size;
	vs_out.vertexID = gl_VertexID;
}
