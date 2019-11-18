#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT{
	vec4 position;
	vec4 normal;
} vs_in[];


void main()
{
    gl_Position = vs_in[0].position; 
    EmitVertex();

    gl_Position = vs_in[0].normal;
    EmitVertex();

    EndPrimitive();
}