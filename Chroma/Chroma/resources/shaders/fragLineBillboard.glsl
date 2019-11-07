#version 330 core
out vec4 FragColor;
in vec2 TexCoords;


void main()
{             


    FragColor = vec4(max(TexCoords.x,0.1), 0.0 ,0.0, 1.0);

}