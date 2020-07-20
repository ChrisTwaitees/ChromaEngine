#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

in VS_OUT{
	mat4 MVPMat;
	float size;
} vs_in[];

out GS_OUT{
	vec4 gs_color;
} gs_out;



void createCone(mat4 mvpMat, vec4 color, vec4 startPos, vec3 endPos, float size)
{
	gs_out.gs_color = color;
	gl_Position = mvpMat * vec4(0.0,0.0,0.0,1.0) ;
	EmitVertex();
	gl_Position = mvpMat * vec4(endPos * size,1.0);
	EmitVertex();
	gl_Position = mvpMat * vec4(0.0,1.0,1.0,1.0) ;
	EmitVertex();
	EndPrimitive();
}


void CreatePlane(mat4 mvpMat, vec3 firstAxis, vec3 secondAxis, float size)
{
	vec3 scale = vec3(0.2 * size);

	vec3 midPoint  = mix(firstAxis, secondAxis, vec3(0.5));
	gs_out.gs_color = vec4(midPoint, 0.5);

	gl_Position =  mvpMat * (  vec4(0.0, 0.0, 0.0, 1.0));    // 1:bottom-left
    EmitVertex();   						
    gl_Position =  mvpMat * (  vec4(firstAxis * scale, 1.0));    // 2:bottom-right
    EmitVertex();  			 					
    gl_Position =  mvpMat * (  vec4(secondAxis * scale, 1.0));    // 3:top-left
    EmitVertex();  					
    gl_Position =  mvpMat * (  vec4( midPoint * vec3(2.0) * scale, 1.0));    // 4:top-right
    EmitVertex();  								 
    EndPrimitive();
}

void main()
{
	// Construction attrs 
	mat4 MVPMat = vs_in[0].MVPMat;
	float size = vs_in[0].size;

	// axis
	vec3 xAxis = vec3(1.0, 0.0, 0.0);
	vec3 yAxis = vec3(0.0, 1.0, 0.0);
	vec3 zAxis = vec3(0.0, 0.0, 1.0);

	// COORDINATE REFERENCE
	// X
	// +
	CreatePlane(MVPMat,  xAxis, yAxis, size);

	// Y
	// +
	CreatePlane(MVPMat,  yAxis, zAxis, size);

	// Z
	// +
	CreatePlane(MVPMat, xAxis, zAxis, size);
}