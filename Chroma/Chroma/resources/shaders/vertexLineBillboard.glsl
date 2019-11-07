#version 330 core
layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 aNormal;
layout (location = 2 ) in vec2 aTexCoords;

out vec2 TexCoords;

uniform vec3 start;
uniform vec3 end;
uniform float thickness;


uniform mat4 projection;
uniform mat4 view;


void main()
{
	TexCoords = aTexCoords;
	mat4 viewMat = inverse(view);
	vec3 cameraPos = vec3(viewMat[3][0], viewMat[3][1], viewMat[3][2]);
	vec3 Pos;
	                 // start      // end
	(aPos.x < 0.0) ? Pos = start : Pos = end;
	// building new normal
	vec3 tangent = normalize(start - end);
	vec3 up = normalize(Pos - cameraPos);

	vec3 newNormal;
	float width = max(thickness *  length(Pos - cameraPos) * 0.01, 0.01);
	// thickness
	if(aPos.y > 0.0)
		newNormal = cross(up, tangent);
	if(aPos.y < 0.0)
		newNormal = cross(tangent, up);

	Pos += newNormal * width;


    gl_Position = projection * view * vec4(Pos, 1.0);
}