#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoords;


out vec2 TexCoords;
uniform vec2 scale;
uniform vec2 offset;

void main()
{
	
	gl_Position = vec4((aPos.x * scale.x) + offset.x, (aPos.y * scale.y) + offset.y, 0.0, 1.0);
	TexCoords = aTexCoords;
}