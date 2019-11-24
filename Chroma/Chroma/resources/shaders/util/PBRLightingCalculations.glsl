// DIRECTIONAL LIGHT
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace, float SSAO, sampler2D shadowmap)
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
	
	vec3 amb = albedo * ambient * SSAO;
	// shadows
	float shadow = ShadowCalculation(FragPosLightSpace, shadowmap, normal, lightDir);

	return (amb + (1.0 - shadow) * (diffuse + spec)) * albedo; 
}

// POINT LIGHT
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo, float roughness, float specular, float metalness, float ambient, vec4 FragPosLightSpace, float SSAO, sampler2D shadowmap)
{
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse
	vec3 diffuse = albedo * light.diffuse * max(dot(lightDir, normal), 0.0);
	// specular  light
	vec3 halfwayDir = normalize(lightDir + viewDir);
	// spec
	vec3 spec = specular * pow(max(dot(normal, halfwayDir), 0.0), roughness) * vec3(light.intensity);
	// ambient
	vec3 amb = albedo * ambient * SSAO;
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	diffuse *=  attenuation * light.intensity;
	spec *= attenuation * light.intensity;
	amb *= attenuation * light.intensity;
	// shadows
	float shadow = ShadowCalculation(FragPosLightSpace, shadowmap, normal, lightDir) ;

	// return 
	return amb + ((1.0 - shadow) * (diffuse + spec)) * albedo;
}


float ShadowCalculation(vec4 FragPosLightSpace, sampler2D shadowmap, vec3 normal, vec3 lightDir)
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
    float bias = max(0.03 * (1.0 - dot(normal, lightDir)), 0.003);
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