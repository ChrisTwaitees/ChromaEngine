// Author:	Fredrik Präntare <prantare@gmail.com> 
// Date:	11/26/2016
#version 450 core

layout(location = 0) in vec3 aPos;

out VS_OUT{
	out vec2 textureCoordinateFrag; 
} vs_out;

// Scales and bias a given vector (i.e. from [-1, 1] to [0, 1]).
vec2 scaleAndBias(vec2 p) { return 0.5f * p + vec2(0.5f); }

void main(){
	vs_out.textureCoordinateFrag = scaleAndBias(aPos.xy);
	gl_Position = vec4(aPos, 1);
}