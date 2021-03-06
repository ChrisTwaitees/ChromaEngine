#version 330 core
layout (points) in;
layout (line_strip, max_vertices=2) out;

// matrix uniforms
uniform mat4 model;
#include "util/uniformBufferCamera.glsl"


void main() {    
    gl_Position = projection * view * model * gl_in[0].gl_Position ; 
    EmitVertex();

    gl_Position = projection * view * model * gl_in[1].gl_Position;
    EmitVertex();
    
    EndPrimitive();
}