// Simple non-conservative voxelization.
// Implementation inspired by Cheng-Tso Lin: 
// https://github.com/otaku690/SparseVoxelOctree/blob/master/WIN/SVO/shader/voxelize.geom.glsl.

#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec3 worldPositionGeom;
	vec3 normalGeom;
	vec2 texCoordsGeom;
} vs_in[];

out GS_OUT{
	vec3 worldPositionFrag;
	vec3 normalFrag;
	vec2 texCoordsFrag;
} gs_out;

void main(){
	const vec3 p1 = vs_in[1].worldPositionGeom - vs_in[0].worldPositionGeom;
	const vec3 p2 = vs_in[2].worldPositionGeom - vs_in[0].worldPositionGeom;
	const vec3 p = abs(cross(p1, p2)); 
	for(uint i = 0; i < 3; ++i)
	{
		gs_out.worldPositionFrag = vs_in[i].worldPositionGeom;
		gs_out.normalFrag = vs_in[i].normalGeom;
		gs_out.texCoordsFrag = vs_in[i].texCoordsGeom;
		if(p.z > p.x && p.z > p.y){
			gl_Position = vec4(gs_out.worldPositionFrag.x, gs_out.worldPositionFrag.y, 0, 1);
		} else if (p.x > p.y && p.x > p.z){
			gl_Position = vec4(gs_out.worldPositionFrag.y, gs_out.worldPositionFrag.z, 0, 1);
		} else {
			gl_Position = vec4(gs_out.worldPositionFrag.x, gs_out.worldPositionFrag.z, 0, 1);
		}
		EmitVertex();
	}
    EndPrimitive();
}