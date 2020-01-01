#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 19) out;

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
	float jointLength = length(origin - child);

	// COORDINATE REFERENCE
	// X
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0) ;
	EmitVertex();
	gl_Position = MVPMat * (vec4(1.0,0.0,0.0,1.0) * vec4(size * 2.0) );
	EmitVertex();
	EndPrimitive();
	gs_out.X = true;
	// Y
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();
	gl_Position = MVPMat * (vec4(0.0,1.0,0.0,1.0) * vec4(size * 2.0));
	EmitVertex();
	EndPrimitive();
	gs_out.Y = true;
	// Z
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();
	gl_Position = MVPMat * (vec4(0.0,0.0,1.0,1.0) * vec4(size * 2.0));
	EmitVertex();
	EndPrimitive();
	gs_out.Z = true;


	// Joint Pyramid Shape
	gs_out.Joint = true; // coloring
	vec4 pyramidCenter = vec4(0.0,jointLength,0.0,1.0) ;
	vec4 topLeft       = vec4(-1.0,0.0,-1.0,1.0);
	vec4 topRight      = vec4(1.0,0.0,-1.0,1.0);
	vec4 bottomLeft    = vec4(-1.0,0.0,1.0,1.0);
	vec4 bottomRight   = vec4(1.0,0.0,1.0,1.0);


	// Create Square
	// top left
	gl_Position =  MVPMat * topLeft;
	EmitVertex();

	// bottom left
	gl_Position =  MVPMat * bottomLeft;
	EmitVertex();

	// bottom right 
	gl_Position =  MVPMat * bottomRight;
	EmitVertex();

	// top right 
	gl_Position =  MVPMat * topRight;
	EmitVertex();

	// top left
	gl_Position =  MVPMat * topLeft;
	EmitVertex();
	// Build Pyramid

	// point
	gl_Position =  MVPMat * pyramidCenter;
	EmitVertex();

	// bottom left
	gl_Position =  MVPMat * bottomLeft;
	EmitVertex();

	// point
	gl_Position =  MVPMat * pyramidCenter;
	EmitVertex();

	// bottom right 
	gl_Position =  MVPMat * bottomRight;
	EmitVertex();

	// point
	gl_Position =  MVPMat * pyramidCenter;
	EmitVertex();

	// top right 
	gl_Position =  MVPMat * topRight;
	EmitVertex();

	// point
	gl_Position =  MVPMat * pyramidCenter;
	EmitVertex();

	// top left
	gl_Position =  MVPMat * topLeft;
	EmitVertex();

	EndPrimitive();



}