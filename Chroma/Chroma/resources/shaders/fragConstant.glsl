#version 330 core
out vec4 FragColor;

uniform float opacity = 0.5f;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform float lightIntensity = 1.0;

void main()
{
    FragColor = vec4(lightColor * lightIntensity, 1.0);
}