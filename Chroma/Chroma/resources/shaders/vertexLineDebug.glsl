#version 330 core


out VS_OUT{
	vec4 position;
	vec4 normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 Start;
uniform vec3 End;

void main()
{
	gl_Position = projection * view * model * vec4(Start, 1.0); 
	vs_out.position = gl_Position;
	vs_out.normal = projection * view * model * vec4(End, 1.0); 
}