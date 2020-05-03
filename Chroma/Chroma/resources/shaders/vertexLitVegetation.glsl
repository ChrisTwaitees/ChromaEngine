#version 330 core
layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 aNormal;
layout (location = 2 ) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  
layout (location = 7) in vec4 aColor;
layout (location = 8) in vec2 aTexCoords2;

out VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
	mat3 TBN;
	vec4 Color;
} vs_out;

vec4 localPosition;

// UNIFORMS
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
// Vegetation Features
uniform float gameTime;
uniform sampler2D noise;
uniform vec2 windFrequency;
uniform vec3 windDirection;
uniform float windSpeed;
uniform float windStrength;


void main()
{    

	//float noiseAmount = (texture(noise, mod((vec2(gameTime) + aPos.xz * noiseFrequency), 1.0)).r * 2) - 1.0;
	float macroNoise = sin(gameTime * windSpeed);
	float microNoise =  (texture(noise, aPos.xz + mod(vec2(gameTime * windFrequency), 1.0)).r * 2 - 1.0) ;
	
	localPosition = vec4(aPos + ((normalize(windDirection) * windStrength * aColor.rrr * macroNoise * microNoise)), 1.0);
	//localPosition = vec4(aPos + ((normalize(windDirection) * sin(gameTime * noiseFrequency.x) * aColor.rrr )), 1.0);
    vs_out.FragPos = vec3(model * localPosition);
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4( vs_out.FragPos , 1.0);
	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));

	vs_out.TBN = mat3(T, B, N);

	vs_out.Color = aColor;



    gl_Position = projection * view * model * localPosition;
}