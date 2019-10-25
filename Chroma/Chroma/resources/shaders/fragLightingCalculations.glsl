// DIRECTIONAL LIGHT
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseMap, vec3 specMap)
{
	// diffuse
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir, normal), 0.0) * light.intensity;
	vec3 diffuse = diff * light.diffuse;
	// specular - light
	vec3 halfwayDir = normalize(lightDir + viewDir);
	// specular  cubemap
	vec3 cubemapDir = normalize(reflect(viewDir, normal));
	vec4 cubemapTex = vec4(texture( material.skybox, cubemapDir ).rgb, 1.0);
	// spec
	vec3 specular = specMap * pow(max(dot(normal, halfwayDir), 0.0), material.roughness) * material.specularIntensity * light.intensity;
	specular = cubemapTex.rgb * specular ;
	// ambient
	vec3 ambient = diffuseMap * material.ambientBrightness * light.intensity;
	// shadows
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);

	return (ambient + (1.0 - shadow) * (diffuse + specular)) * diffuseMap; 
}

// POINT LIGHT
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos,  vec3 diffuseMap, vec3 specMap)
{
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse
	vec3 diffuse = diffuseMap * light.diffuse * max(dot(lightDir, normal), 0.0);
	// specular  light
	vec3 halfwayDir = normalize(lightDir + viewDir);
	// specular  cubemap
	vec3 cubemapDir = normalize(reflect(viewDir, normal));
	vec4 cubemapTex = vec4(texture( material.skybox, cubemapDir ).rgb, 1.0);
	// spec
	vec3 specular = specMap * pow(max(dot(normal, halfwayDir), 0.0), material.roughness) * material.specularIntensity * light.intensity;
	specular = cubemapTex.rgb * specular ;
	// ambient
	vec3 ambient = diffuseMap * material.ambientBrightness;
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	diffuse *=  attenuation * light.intensity;
	specular *= attenuation * light.intensity;
	ambient *= attenuation * light.intensity;
	// shadows
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);

	// return 
	return (ambient + (1.0 - shadow) * (diffuse + specular)) * diffuseMap; 
}

// SPOT LIGHT - not currently implemented
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos,  vec3 diffuseMap, vec3 specMap)
{
	// lightDir
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse
	float theta = dot(lightDir, -viewDir);
	float lit = clamp(1.0 - smoothstep(light.penumbraSize, light.spotSize, theta), 0.0, 1.0);
	vec3 diffuse = diffuseMap * light.diffuse * lit;
	// specular 
	vec3 reflectedLight = normalize(reflect(-lightDir, normal));
	vec3 specular = specMap * pow(max(dot(reflectedLight, viewDir), 0.0), material.roughness) * material.specularIntensity * lit;
	// ambient
	vec3 ambient = diffuseMap * material.ambientBrightness ;
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	diffuse *= attenuation * light.intensity;
	specular *= attenuation * light.intensity;
	ambient *= attenuation * light.intensity;

	// shadows
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);

	// return 
	return (ambient + (1.0 - shadow) * (diffuse + specular)) * diffuseMap; 
}

float ShadowCalculation(vec4 FragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divnride
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowmaps.shadowmap1, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowmaps.shadowmap1, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowmaps.shadowmap1, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;

}