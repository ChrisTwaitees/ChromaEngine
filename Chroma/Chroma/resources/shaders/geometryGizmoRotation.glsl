#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 96) out;

//const 
const float PI = 3.1415926f;
const int numSegments = 32;
const float relativeThickness = 0.025;
const float relativeConeHeight = 0.1;

// axis
const vec3 xAxis = vec3(1.0, 0.0, 0.0);
const vec3 yAxis = vec3(0.0, 1.0, 0.0);
const vec3 zAxis = vec3(0.0, 0.0, 1.0);


in VS_OUT{
	mat4 MVPMat;
	int vertexID;
	float size;
} vs_in[];

out GS_OUT{
	vec4 gs_color;
} gs_out;





void CreateCircle(mat4 mvpMat, vec3 axis, float size)
{
	gs_out.gs_color = vec4(axis, 1.0);


	for (int i = 1 ; i < numSegments + 1 ; i++ )
	{	
		float angle = (PI / float(numSegments*0.5)) * float(i);
		float angle2 = (PI / float(numSegments*0.5)) * float(i+1);


		// identify which access to affect
		if (axis.x > 0.0)
		{

			gl_Position = mvpMat * vec4(0.0,cos(angle) * size,sin(angle)* size,1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(0.0,cos(angle2)* size,sin(angle2)* size,1.0) ;
			EmitVertex();
			EndPrimitive();

			continue;
		}
		if (axis.y > 0.0)
		{
			gl_Position = mvpMat * vec4(cos(angle)* size,0.0,sin(angle)* size,1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* size,0.0,sin(angle2)* size,1.0) ;
			EmitVertex();
			EndPrimitive();

			continue;
		}
		if (axis.z > 0.0)
		{
			gl_Position = mvpMat * vec4(cos(angle)* size,sin(angle)* size, 0.0,1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* size,sin(angle2)* size, 0.0,1.0) ; 
			EmitVertex();
			EndPrimitive();

			continue;
		}
	}

}

 
void main()
{
	// Construction attrs 
	mat4 MVPMat = vs_in[0].MVPMat;
	float size = vs_in[0].size;

	// X
	if(vs_in[0].vertexID == 0)
	{
		CreateCircle(MVPMat, xAxis, size);
	}
	
	// Y
	if(vs_in[0].vertexID == 1)
	{
		CreateCircle(MVPMat, yAxis, size);
	}

	// Z
	if(vs_in[0].vertexID == 2)
	{
		CreateCircle(MVPMat, zAxis, size);
	}
}