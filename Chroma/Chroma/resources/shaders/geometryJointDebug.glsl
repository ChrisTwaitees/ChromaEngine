#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 8) out;

in VS_OUT{
	vec3 originPos;
	vec3 childPos;
	mat4 MVPMat;
	float size;
} vs_in[];

out GS_OUT{
	bool X;
	bool Y;
	bool Z;
	bool Joint;
} gs_out;

void main()
{
	// Construction attrs 
	vec4 origin = vec4(vs_in[0].originPos, 1.0);
	vec4 child = vec4(vs_in[0].childPos, 1.0);
	mat4 MVPMat = vs_in[0].MVPMat;
	float size = vs_in[0].size;
	float jointLength = length(child - origin);

	// COORDINATE REFERENCE
	// X
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();
	gl_Position = MVPMat * vec4(1.0,0.0,0.0,1.0);
	EmitVertex();
	EndPrimitive();
	gs_out.X = true;
	// Y
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();
	gl_Position = MVPMat * vec4(0.0,1.0,0.0,1.0);
	EmitVertex();
	EndPrimitive();
	gs_out.Y = true;
	// Z
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();
	gl_Position = MVPMat * vec4(0.0,0.0,1.0,1.0);
	EmitVertex();
	EndPrimitive();
	gs_out.Z = true;



	// BASE
	// origin to childPos
	gs_out.Joint = true;
	gs_out.X = false;
	gs_out.Y = false;
	gs_out.Z = false;
	gl_Position =  MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();

	gl_Position =  MVPMat * vec4(0.0,jointLength,0.0,1.0);
	EmitVertex();

	EndPrimitive();



}