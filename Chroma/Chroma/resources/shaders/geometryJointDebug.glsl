#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT{
	vec3 originPos;
	vec3 childPos;
	mat4 MVPMat;
} vs_in[];


void main()
{
	// from vtx 
	vec4 origin = vec4(vs_in[0].originPos, 1.0);
	vec4 child = vec4(vs_in[0].childPos, 1.0);
	mat4 MVPMat = vs_in[0].MVPMat;


	// origin to childPos
	gl_Position = MVPMat * origin;
	EmitVertex();

	gl_Position = MVPMat * child;
	EmitVertex();

	EndPrimitive();

}