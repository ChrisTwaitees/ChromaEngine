#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 13) out;

in VS_OUT{
	vec3 jointPos;
	vec3 childPos;
	mat4 VPMat;
	mat4 jointMat;
	float size;
} vs_in[];


void main()
{
	// Construction attrs 
	vec3 jointPos = vs_in[0].jointPos;
	vec4 childPos = vec4(vs_in[0].childPos, 1.0);
	mat4 VPMat = vs_in[0].VPMat;
	mat4 jointMat = vs_in[0].jointMat;
	float size = vs_in[0].size;

	// COORDINATE REFERENCE
	// X
	//vec4 X  = normalize(jointMat * vec4(1.0,0.0,0.0,1.0));
	// Z
	//vec4 Z  = normalize(jointMat * vec4(0.0,0.0,1.0,1.0));
	vec3 Y = normalize(jointPos - childPos.xyz) * 0.05 * size;

	vec3 X = normalize(cross(Y, vec3(0.0,1.0,0.0)))* 0.05 * size;

	vec3 Z = normalize(cross(Y, X)) * 0.05 * size;

	// Pyramid Shape
	vec4 topLeft       = vec4(jointPos + ( -X - Z ) , 1.0);
	vec4 topRight      = vec4(jointPos + (  X - Z ), 1.0);
	vec4 bottomLeft    = vec4(jointPos + ( -X + Z ), 1.0);
	vec4 bottomRight   = vec4(jointPos + (  X + Z ), 1.0);
	vec4 pyramidCenter =  childPos;

	// Create Square
	// top left
	gl_Position =  VPMat * topLeft;
	EmitVertex();

	// bottom left
	gl_Position =  VPMat * bottomLeft;
	EmitVertex();

	// bottom right 
	gl_Position =  VPMat * bottomRight;
	EmitVertex();

	// top right 
	gl_Position =  VPMat * topRight;
	EmitVertex();

	// top left
	gl_Position =  VPMat * topLeft;
	EmitVertex();
	// Build Pyramid

	// point
	gl_Position =  VPMat * pyramidCenter;
	EmitVertex();

	// bottom left
	gl_Position =  VPMat * bottomLeft;
	EmitVertex();

	// point
	gl_Position =  VPMat * pyramidCenter;
	EmitVertex();

	// bottom right 
	gl_Position =  VPMat * bottomRight;
	EmitVertex();

	// point
	gl_Position =  VPMat * pyramidCenter;
	EmitVertex();

	// top right 
	gl_Position =  VPMat * topRight;
	EmitVertex();

	// point
	gl_Position =  VPMat * pyramidCenter;
	EmitVertex();

	// top left
	gl_Position =  VPMat * topLeft;
	EmitVertex();

	EndPrimitive();



}