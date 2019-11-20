#version 330 core
layout (points) in;
layout (points, max_vertices = 1) out;

out VS_OUT{
	float radius;
} vs_in[];


void main()
{
    gl_Position = gl_in[0].gl_Position;
	gl_PointSize = vs_in[0].radius;
    EmitVertex();

    EndPrimitive();
}