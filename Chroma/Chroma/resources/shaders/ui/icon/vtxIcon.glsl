#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

// matrix uniforms
uniform mat4 modelView;
uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoords;

void main()
{
	gl_Position = projection * modelView * aPos;
	TexCoords = aTexCoords;
}
