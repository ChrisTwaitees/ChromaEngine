#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 128) out;

//const 
const float PI = 3.1415926f;
const int numSegments = 8;
const float relativeThickness = 0.025;
const float relativeConeHeight = 0.1;
const vec4 selectedColor = vec4(1.0, 1.0, 0.0, 1.0);
const float hoveredOpacity = 0.5;

// axis
const vec3 xAxis = vec3(1.0, 0.0, 0.0);
const vec3 yAxis = vec3(0.0, 1.0, 0.0);
const vec3 zAxis = vec3(0.0, 0.0, 1.0);

// uniforms
uniform bool u_XAxisEnabled;
uniform bool u_YAxisEnabled;
uniform bool u_ZAxisEnabled;

uniform bool u_XAxisHovered;
uniform bool u_YAxisHovered;
uniform bool u_ZAxisHovered;


in VS_OUT{
	mat4 MVPMat;
	int vertexID;
	float size;
} vs_in[];

out GS_OUT{
	vec4 gs_color;
} gs_out;



void CreateCone(mat4 mvpMat, vec3 axis, float size)
{
	float coneWidth = size * relativeThickness * 2.0; 
	// shrink for cone
	float coneEnd = size;
	float coneStart =  size - ( size * relativeConeHeight);

	for (int i = 1 ; i < numSegments + 1 ; i++ )
	{	
	
		float angle = (PI / float(numSegments*0.5)) * float(i);
		float angle2 = (PI / float(numSegments*0.5)) * float(i+1);

		if(axis.x > 0.0)
		{
			if(u_XAxisEnabled)
				gs_out.gs_color = selectedColor;

			if(u_XAxisHovered)
				gs_out.gs_color.w = hoveredOpacity;

			gl_Position = mvpMat * vec4(coneStart, cos(angle)* coneWidth,sin(angle)* coneWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(coneStart,  cos(angle2)* coneWidth,sin(angle2)* coneWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(coneEnd, 0.0 , 0.0, 1.0) ; 
			EmitVertex();

			EndPrimitive();
		}

		if(axis.y > 0.0)
		{
			if(u_YAxisEnabled)
				gs_out.gs_color = selectedColor;

			if(u_YAxisHovered)
				gs_out.gs_color.w = hoveredOpacity;

			gl_Position = mvpMat * vec4(cos(angle)* coneWidth, coneStart, sin(angle)* coneWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* coneWidth, coneStart, sin(angle2)* coneWidth, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(0.0, coneEnd , 0.0, 1.0) ; 
			EmitVertex();

			EndPrimitive();
		}

		if(axis.z > 0.0)
		{
			if(u_ZAxisEnabled)
				gs_out.gs_color = selectedColor;

			if(u_ZAxisHovered)
				gs_out.gs_color.w = hoveredOpacity;

			gl_Position = mvpMat * vec4(cos(angle)* coneWidth,sin(angle)* coneWidth, coneStart, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* coneWidth,sin(angle2)* coneWidth, coneStart, 1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(0.0, 0.0 , coneEnd, 1.0) ; 
			EmitVertex();

			EndPrimitive();
		}

	}
}


void CreateCylinderCaps(mat4 mvpMat, vec3 axis, float size)
{
	float capWidth = size * relativeThickness; 
	// shrink for cone
	size -= size * relativeConeHeight;
	// top cap
	for (int i = 1 ; i < numSegments + 1 ; i++ )
	{	
	
		float angle = (PI / float(numSegments*0.5)) * float(i);
		float angle2 = (PI / float(numSegments*0.5)) * float(i+1);

		gl_Position = mvpMat * vec4(0.0,0.0,0.0,1.0) ; // center point
		EmitVertex();

		// identify which access to affect
		if (axis.x > 0.0)
		{

			if(u_XAxisEnabled)
				gs_out.gs_color = selectedColor;

			if(u_XAxisHovered)
				gs_out.gs_color.w = hoveredOpacity;

			gl_Position = mvpMat * vec4(0.0,cos(angle) * capWidth,sin(angle)* capWidth,1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(0.0,cos(angle2)* capWidth,sin(angle2)* capWidth,1.0) ;
			EmitVertex();
			EndPrimitive();

			// top cap
			gl_Position = mvpMat * vec4(axis * size ,1.0) ; // center point
			EmitVertex();

			gl_Position = mvpMat * vec4(size, cos(angle)* capWidth, sin(angle)* capWidth,1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(size, cos(angle2)* capWidth, sin(angle2)* capWidth,1.0) ;
			EmitVertex();
			EndPrimitive();
			continue;
		}
		if (axis.y > 0.0)
		{
			if(u_YAxisEnabled)
				gs_out.gs_color = selectedColor;

			if(u_YAxisHovered)
				gs_out.gs_color.w = hoveredOpacity;

			gl_Position = mvpMat * vec4(cos(angle)* capWidth,0.0,sin(angle)* capWidth,1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* capWidth,0.0,sin(angle2)* capWidth,1.0) ;
			EmitVertex();
			EndPrimitive();

			// top cap
			gl_Position = mvpMat * vec4(axis * size ,1.0) ; // center point
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle)* capWidth, size, sin(angle)* capWidth,1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* capWidth, size, sin(angle2)* capWidth,1.0) ;
			EmitVertex();
			EndPrimitive();
			continue;
		}
		if (axis.z > 0.0)
		{

			if(u_ZAxisEnabled)
				gs_out.gs_color = selectedColor;

			if(u_ZAxisHovered)
				gs_out.gs_color.w = hoveredOpacity;

			gl_Position = mvpMat * vec4(cos(angle)* capWidth,sin(angle)* capWidth, 0.0,1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* capWidth,sin(angle2)* capWidth, 0.0,1.0) ; 
			EmitVertex();
			EndPrimitive();

			// top cap
			gl_Position = mvpMat * vec4(axis * size ,1.0) ; // center point
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle)* capWidth, sin(angle)* capWidth, size,1.0) ; 
			EmitVertex();

			gl_Position = mvpMat * vec4(cos(angle2)* capWidth, sin(angle2)* capWidth, size,1.0) ;
			EmitVertex();
			EndPrimitive();
			continue;
		}
	}
}

 

void CreateCylinder(mat4 mvpMat, vec3 axis, float size)
{
	// color
	gs_out.gs_color = vec4(axis, 1.0);

	// caps
	CreateCylinderCaps(mvpMat, axis, size);

	// cone
	CreateCone( mvpMat, axis, size);

	// shrink for cone
	size -= size * relativeConeHeight;

	// cylinder
	float cylWidth = size * relativeThickness; 
	for (int i = 1 ; i < numSegments + 1 ; i++ )
	{	
	
		float angle = (PI / float(numSegments*0.5)) * float(i);
		float angle2 = (PI / float(numSegments*0.5)) * float(i+1);

		if(axis.x > 0.0)
		{
			if(u_XAxisEnabled)
				gs_out.gs_color = selectedColor;

			if(u_XAxisHovered)
				gs_out.gs_color.w = hoveredOpacity;

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
			if(u_YAxisEnabled)
				gs_out.gs_color = selectedColor;

			if(u_YAxisHovered)
				gs_out.gs_color.w = hoveredOpacity;

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
			if(u_ZAxisEnabled)
				gs_out.gs_color = selectedColor;

			if(u_ZAxisHovered)
				gs_out.gs_color.w = hoveredOpacity;

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