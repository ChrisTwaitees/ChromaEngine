#version 330 core
out vec4 FragColor;


// in
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;



struct Material
{
	float  cubemapIntensity;
	samplerCube skybox;
};

uniform vec3 viewPos;
uniform Material material;

vec3 calcRefraction(vec3 FragPos, vec3 viewPos, vec3 Normal)
{
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(FragPos - viewPos);
	return  refract(I, normalize(Normal), ratio);
}

void main() 
{
	vec3 refractionDir = calcRefraction(FragPos, viewPos, Normal);
	FragColor = vec4(texture(material.skybox, refractionDir).rgb, 1.0); 
}