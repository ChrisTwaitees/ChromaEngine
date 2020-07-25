#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 128) out;

//const 
const float PI = 3.1415926f;
const int numSegments = 8;
const float relativeThickness = 0.025;
const float relativeCubeHeight = 0.1;

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



void CreateCube(mat4 mvpMat, vec3 axis, float size)
{
	float boxSize = size * relativeThickness * 2.0; 
	axis *= 1.0 - boxSize;
	axis *= size;

	gl_Position = mvpMat * vec4((vec3(1.0 * boxSize, 1.0 * boxSize, 0.0) + axis)  , 1.0); // top right
	EmitVertex();

	gl_Position = mvpMat * vec4((vec3(-1.0 * boxSize, 1.0 * boxSize, 0.0) + axis)  , 1.0); // top left
	EmitVertex();

	gl_Position = mvpMat * vec4((vec3(1.0 * boxSize, -1.0 * boxSize, 0.0)  + axis)  , 1.0); // bottom right
	EmitVertex();

	gl_Position = mvpMat * vec4((vec3(-1.0 * boxSize, -1.0 * boxSize, 0.0) + axis)  , 1.0); // bottom left
	EmitVertex();

	EndPrimitive();


}

 

void CreateCylinder(mat4 mvpMat, vec3 axis, float size)
{
	// color
	gs_out.gs_color = vec4(axis, 1.0);

	// cube
	CreateCube( mvpMat, axis, size);

	// shrink for cone
	size -= size * relativeCubeHeight;

	// cylinder
	float cylWidth = size * relativeThickness; 
	for (int i = 1 ; i < numSegments + 1 ; i++ )
	{	
	
		float angle = (PI / float(numSegments*0.5)) * float(i);
		float angle2 = (PI / float(numSegments*0.5)) * float(i+1);

		if(axis.x > 0.0)
		{
			gl_Position = mvpMat * vec4(0.0, cos(angle)* cylWidth,sin(angle)* cylWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(0.0,  cos(angle2)* cylWidth,sin(angle2)* cylWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(size, cos(angle)* cylWidth,sin(angle)* cylWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(size, cos(angle2)* cylWidth,sin(angle2)* cylWidth, 1.0) ; 
			EmitVertex();
			EndPrimitive();
		}


		if(axis.y > 0.0)
		{
			gl_Position = mvpMat * vec4(cos(angle)* cylWidth, 0.0, sin(angle)* cylWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* cylWidth, 0.0, sin(angle2)* cylWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle)* cylWidth, size, sin(angle)* cylWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* cylWidth, size, sin(angle2)* cylWidth, 1.0) ; 
			EmitVertex();
			EndPrimitive();
		}



		if(axis.z > 0.0)
		{
			gl_Position = mvpMat * vec4(cos(angle)* cylWidth,sin(angle)* cylWidth, 0.0, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* cylWidth,sin(angle2)* cylWidth, 0.0, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle)* cylWidth,sin(angle)* cylWidth, size, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* cylWidth,sin(angle2)* cylWidth, size, 1.0) ; 
			EmitVertex();
			EndPrimitive();
		}
	}


}

void CreatePlane(mat4 mvpMat, vec3 firstAxis, vec3 secondAxis, float size)
{
	vec3 scale = vec3(size * 0.25);

	vec3 midPoint  = mix(firstAxis, secondAxis, vec3(0.5));
	gs_out.gs_color = vec4(midPoint*2.0, 0.75);
	
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

	// X
	if(vs_in[0].vertexID == 0)
	{
		CreatePlane(MVPMat,  xAxis, yAxis, size);
		CreateCylinder(MVPMat, xAxis, size);
	}
	
	// Y
	if(vs_in[0].vertexID == 1)
	{
		CreatePlane(MVPMat,  yAxis, zAxis, size);
		CreateCylinder(MVPMat, yAxis, size);
	}

	// Z
	if(vs_in[0].vertexID == 2)
	{
		CreatePlane(MVPMat, xAxis, zAxis, size);
		CreateCylinder(MVPMat, zAxis, size);
	}
}