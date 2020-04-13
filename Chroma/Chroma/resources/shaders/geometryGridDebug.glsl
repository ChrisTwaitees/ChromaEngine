#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 250) out;

in VS_OUT{
	mat4 MVPMat;
	float size;
} vs_in[];


void main()
{
	// Construction attrs 
	mat4 MVPMat = vs_in[0].MVPMat;
	int size = int(vs_in[0].size / 2.0);

	// iter
	for(int i = 0; i <= size ; i++)
	{
		// origin
		if(i == 0)
		{
		// row
			gl_Position = MVPMat * vec4(size,0.0,0.0,1.0) ;
			EmitVertex();
			gl_Position = MVPMat * vec4(-size,0.0,0.0,1.0) ;
			EmitVertex();
			EndPrimitive();
		//colums
			gl_Position = MVPMat * vec4(0.0,0.0,size,1.0) ;
			EmitVertex();
			gl_Position = MVPMat * vec4(0.0,0.0,-size,1.0) ;
			EmitVertex();
			EndPrimitive();
		}
		else
		{
		// row
			gl_Position = MVPMat * vec4(size,0.0,i,1.0) ;
			EmitVertex();
			gl_Position = MVPMat * vec4(-size,0.0,i,1.0) ;
			EmitVertex();
			EndPrimitive();

			gl_Position = MVPMat * vec4(size,0.0,-i,1.0) ;
			EmitVertex();
			gl_Position = MVPMat * vec4(-size,0.0,-i,1.0) ;
			EmitVertex();
			EndPrimitive();

		// column
			gl_Position = MVPMat * vec4(i,0.0,size,1.0) ;
			EmitVertex();
			gl_Position = MVPMat * vec4(i,0.0,-size,1.0) ;
			EmitVertex();
			EndPrimitive();

			gl_Position = MVPMat * vec4(-i,0.0,size,1.0) ;
			EmitVertex();
			gl_Position = MVPMat * vec4(-i,0.0,-size,1.0) ;
			EmitVertex();
			EndPrimitive();
		}

	}
}