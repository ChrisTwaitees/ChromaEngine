#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
  
in vec2 TexCoords;

// BUFFERS
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoRoughness;
uniform sampler2D gMetalnessSpecular;
uniform sampler2D gFragPosLightSpace;
uniform sampler2D gShadowmap;
uniform sampler2D SSAO;

// LIGHTS
#include "util/lightingStructs.glsl"
#define NR_POINT_LIGHTS 9
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 1

// UNIFORMS
uniform samplerCube skybox;
// Lighting Uniforms
uniform vec3 viewPos;
uniform float ambient;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];


// Lighting Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace, float SSAO, sampler2D shadowmap);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace, float SSAO, sampler2D shadowmap);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos,  vec3 diffuseMap, vec3 specMap);
float ShadowCalculation(vec4 FragPosLightSpace, sampler2D shadowmap, vec3 normal, vec3 lightDir);

void main()
{         
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoRoughness, TexCoords).rgb;
    float Roughness = texture(gAlbedoRoughness, TexCoords).a;
	float Metalness = texture(gMetalnessSpecular, TexCoords).r;
	float Specular = texture(gMetalnessSpecular, TexCoords).a;
	vec4 FragPosLightSpace = texture(gFragPosLightSpace, TexCoords).rgba;
	float ssao = texture(SSAO, TexCoords).r;

	// lighting
	vec3 lighting;
	vec3 viewDir = normalize(viewPos - FragPos);
	
	 //directional lights
	for(int i = 0; i < NR_DIR_LIGHTS ; i++)
		lighting += CalcDirLight(dirLights[i], Normal, viewDir,  Albedo, Roughness, Specular, Metalness, ambient, FragPosLightSpace, ssao, gShadowmap);

	// point lights
	for(int i = 0; i < NR_POINT_LIGHTS ; i++)
	{
	    float dist = length(pointLights[i].position - FragPos);
        if(dist < pointLights[i].radius)  // do expensive lighting
            lighting += CalcPointLight(pointLights[i], Normal, viewDir, FragPos, Albedo, Roughness, Specular, Metalness, ambient, FragPosLightSpace, ssao, gShadowmap);
	}

	// out
	FragColor = vec4(lighting * vec3(ssao), 1.0);

	// out bloom
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}  

#include "util/BlinnPhongLightingCalculations.glsl"