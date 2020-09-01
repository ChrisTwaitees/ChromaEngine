#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;
uniform int voxelDimensions;


// UNIFORMS

in VS_OUT{
	mat4 VPMat;
	vec4 voxelColorGeom;
	float boxSize;
} vs_in[];

out GS_OUT{
	vec4 voxelColorFrag;
} gs_out;

void createVertex(vec3 inPos)
{

	
	vec4 worldPos;
	//worldPos.xyz = gl_in[0].gl_Position.xyz / vec3(voxelDimensions) * 2 - 1.0;
	worldPos = gl_in[0].gl_Position + vec4(vec3(inPos * vs_in[0].boxSize),1.0) ;
	//worldPos.y = -worldPos.y;
	//worldPos.xyz *= voxelDimensions;
	//worldPos.xyz += (inPos - vec3(0,1,0)) * 2.0;
	//worldPos.xyz += inPos;
	//worldPos.xyz *= voxelDimensions * 1.0/ voxelDimensions;

	gl_Position = vs_in[0].VPMat * worldPos;
	
	EmitVertex();
}


void main()
{

	vec4 voxelColor = vs_in[0].voxelColorGeom;
	if(voxelColor.rgb == vec3(0.0))
		return;

	gs_out.voxelColorFrag = vs_in[0].voxelColorGeom;
	
	createVertex(vec3(-1.0, 1.0, 1.0));
	createVertex(vec3(-1.0, -1.0, 1.0));
	createVertex(vec3(1.0, 1.0, 1.0));
	createVertex(vec3(1.0, -1.0, 1.0));
	
	EndPrimitive();
	
	createVertex(vec3(1.0, 1.0, 1.0));
	createVertex(vec3(1.0, -1.0, 1.0));
	createVertex(vec3(1.0, 1.0, -1.0));
	createVertex(vec3(1.0, -1.0, -1.0));
	
	EndPrimitive();
	
	createVertex(vec3(1.0, 1.0, -1.0));
	createVertex(vec3(1.0, -1.0, -1.0));
	createVertex(vec3(-1.0, 1.0, -1.0));
	createVertex(vec3(-1.0, -1.0, -1.0));
	
	EndPrimitive();
	
	createVertex(vec3(-1.0, 1.0, -1.0));
	createVertex(vec3(-1.0, -1.0, -1.0));
	createVertex(vec3(-1.0, 1.0, 1.0));
	createVertex(vec3(-1.0, -1.0, 1.0));
	
	EndPrimitive();
	
	createVertex(vec3(1.0, 1.0, 1.0));
	createVertex(vec3(1.0, 1.0, -1.0));
	createVertex(vec3(-1.0, 1.0, 1.0));
	createVertex(vec3(-1.0, 1.0, -1.0));
	
	EndPrimitive();
	
	createVertex(vec3(-1.0, -1.0, 1.0));
	createVertex(vec3(-1.0, -1.0, -1.0));
	createVertex(vec3(1.0, -1.0, 1.0));
	createVertex(vec3(1.0, -1.0, -1.0));
	
	EndPrimitive();



}