// Author:	Fredrik Präntare <prantare@gmail.com>
// Date:	11/26/2016
#version 450 core

in VS_OUT{
	vec3 worldPosition;
} vs_in;

out vec4 color;

void main(){ color.rgb = vs_in.worldPosition; }
