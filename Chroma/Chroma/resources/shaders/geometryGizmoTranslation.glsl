#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 12) out;

in VS_OUT{
	mat4 MVPMat;
	float size;
} vs_in[];

//out GS_OUT{
//	vec4 gs_color;
//} gs_out;

void main()
{
	// Construction attrs 
	mat4 MVPMat = vs_in[0].MVPMat;
	float Size = vs_in[0].size;

	// COORDINATE REFERENCE
	// X
	// +
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0) ;
	EmitVertex();
	gl_Position = MVPMat * vec4(1.0 * Size,0.0,0.0,1.0);
	EmitVertex();
	EndPrimitive();
	// -
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0) ;
	EmitVertex();
	gl_Position = MVPMat * vec4(-1.0 * Size,0.0,0.0,1.0);
	EmitVertex();
	EndPrimitive();

	// Y
	// +
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();
	gl_Position = MVPMat * vec4(0.0,1.0 * Size,0.0,1.0);
	EmitVertex();
	EndPrimitive();
	// -
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();
	gl_Position = MVPMat * vec4(0.0,-1.0 * Size,0.0,1.0);
	EmitVertex();
	EndPrimitive();

	// Z
	// +
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();
	gl_Position = MVPMat * vec4(0.0,0.0,1.0 * Size ,1.0) ;
	EmitVertex();
	EndPrimitive();
	// -
	gl_Position = MVPMat * vec4(0.0,0.0,0.0,1.0);
	EmitVertex();
	gl_Position = MVPMat * vec4(0.0,0.0,-1.0 * Size ,1.0) ;
	EmitVertex();
	EndPrimitive();


}