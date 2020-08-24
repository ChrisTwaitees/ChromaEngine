// Author:	Fredrik Präntare <prantare@gmail.com>
// Date:	11/26/2016
#version 450 core

layout (location = 0 ) in vec3 aPos;

#include "util/uniformBufferCamera.glsl"
uniform mat4 model;

out VS_OUT{
	out vec3 worldPosition;
} vs_out;


void main(){
	vs_out.worldPosition = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0);

}