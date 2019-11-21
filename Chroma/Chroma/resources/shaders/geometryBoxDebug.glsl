#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 16) out;

in VS_OUT{
	vec3 bbox_min;
	vec3 bbox_max;
	mat4 MVPMat;
} vs_in[];


void main()
{
//	vec4 center = gl_in[0].gl_Position; 
	vec4 bbmin = vec4(vs_in[0].bbox_min, 1.0);
	vec4 bbmax = vec4(vs_in[0].bbox_max, 1.0);

	mat4 MVPMat = vs_in[0].MVPMat;

// bottom front left
    gl_Position =  MVPMat * ( bbmin); 
    EmitVertex();
// bottom front right
    gl_Position =   MVPMat *  ( vec4(bbmax.x, bbmin.y, bbmin.z, 1.0)); 
    EmitVertex();
// bottom back right
    gl_Position =   MVPMat * ( vec4(bbmax.x, bbmin.y, bbmax.z, 1.0)); 
    EmitVertex();
// bottom back left
    gl_Position =   MVPMat * (  vec4(bbmin.x, bbmin.y, bbmax.z, 1.0)); 
    EmitVertex();
// bottom front left
    gl_Position = MVPMat * (  bbmin); 
    EmitVertex();

// top front left
    gl_Position = MVPMat * (  vec4(bbmin.x, bbmax.y, bbmin.z, 1.0 )); 
    EmitVertex();
// top front right
    gl_Position = MVPMat * (  vec4(bbmax.x, bbmax.y, bbmin.z, 1.0 )); 
    EmitVertex();
// bottom front right
    gl_Position =   MVPMat *  ( vec4(bbmax.x, bbmin.y, bbmin.z, 1.0)); 
    EmitVertex();
// top front right
    gl_Position = MVPMat * (  vec4(bbmax.x, bbmax.y, bbmin.z, 1.0 )); 
    EmitVertex();
// top back right
    gl_Position = MVPMat * (  vec4(bbmax.x, bbmax.y, bbmax.z, 1.0 )); 
    EmitVertex();
// bottom back right
    gl_Position =   MVPMat * ( vec4(bbmax.x, bbmin.y, bbmax.z, 1.0)); 
    EmitVertex();
// top back right
    gl_Position = MVPMat * (  vec4(bbmax.x, bbmax.y, bbmax.z, 1.0 )); 
    EmitVertex();
// top back left
    gl_Position = MVPMat * (  vec4(bbmin.x, bbmax.y, bbmax.z, 1.0 )); 
    EmitVertex();
// bottom back left
    gl_Position =   MVPMat * (  vec4(bbmin.x, bbmin.y, bbmax.z, 1.0)); 
    EmitVertex();
// top back left
    gl_Position = MVPMat * (  vec4(bbmin.x, bbmax.y, bbmax.z, 1.0 )); 
    EmitVertex();
// top front left
    gl_Position = MVPMat * (  vec4(bbmin.x, bbmax.y, bbmin.z, 1.0 )); 
    EmitVertex();

    EndPrimitive();
}