#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoRoughness;
uniform sampler2D gMetalnessSpecular;
uniform sampler2D gFragPosLightSpace;


// LIGHTS
struct DirLight 
{
	vec3 direction;
	vec3 diffuse;
	float intensity;
};

struct PointLight
{
	vec3 position;
	vec3 diffuse;
	float intensity;
	// attenuation
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float intensity;
	// attenuation
	float constant;
	float linear;
	float quadratic;
	// spotlight
	float spotSize;
	float penumbraSize;
	vec3 diffuse;
};

// UNIFORMS

// MAX_LIGHTS
#define NR_POINT_LIGHTS 8
#define NR_DIR_LIGHTS 1
#define NR_SPOT_LIGHTS 1

uniform samplerCube skybox;
uniform float skyboxIntensity;

// Lighting Uniforms
uniform vec3 viewPos;
uniform sampler2D shadowmap;
uniform float ambient;

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

// Lighting Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos,  vec3 diffuseMap, vec3 specMap);
float ShadowCalculation(vec4 FragPosLightSpace, vec3 normal, vec3 lightDir);

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

	// lighting
	vec3 lighting;
	vec3 viewDir = normalize(viewPos - FragPos);

	
	// directional lights
	for(int i = 0; i < NR_DIR_LIGHTS ; i++)
		lighting += CalcDirLight(dirLights[i], Normal, viewDir,  Albedo, Roughness, Specular, Metalness, ambient, FragPosLightSpace );

	// point lights
	//for(int i = 0; i < NR_POINT_LIGHTS ; i++)
	//	lighting += CalcPointLight(pointLights[i], Normal, viewDir, FragPos, Albedo, Roughness, Specular, Metalness, ambient, FragPosLightSpace);

	// out
	FragColor = vec4(lighting, 1.0);

	// out bloom
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}  

// DIRECTIONAL LIGHT
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace)
{
	// diffuse
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir, normal), 0.0) * light.intensity;
	vec3 diffuse = diff * light.diffuse;
	// specular - light
	vec3 halfwayDir = normalize(lightDir + viewDir);
	// spec
	vec3 spec = specular * vec3(pow(max(dot(normal, halfwayDir), 0.0), roughness) * light.intensity);
	// ambient
	vec3 amb = albedo * ambient;
	// shadows
	float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);

	//return (amb + (1.0 - shadow) * (diffuse + spec)) * albedo; 
	return (amb + (1.0 ) * (diffuse + spec)) * albedo; 
	//return spec; 
}

// POINT LIGHT
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace)
{
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse
	vec3 diffuse = albedo * light.diffuse * max(dot(lightDir, normal), 0.0);
	// specular  light
	vec3 halfwayDir = normalize(lightDir + viewDir);
	// spec
	vec3 spec = vec3(specular * pow(max(dot(normal, halfwayDir), 0.0), roughness)  * light.intensity);
	// ambient
	vec3 amb = albedo * ambient;
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	diffuse *=  attenuation * light.intensity;
	specular *= attenuation * light.intensity;
	ambient *= attenuation * light.intensity;
	// shadows
	float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);

	// return 
	//return (amb + (1.0 - shadow) * (diffuse + spec)) * albedo; 
	return diffuse; 
}


float ShadowCalculation(vec4 FragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divnride
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowmap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowmap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;

}