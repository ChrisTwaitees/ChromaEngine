// ----------------------------------------------------------------------------
// POINT LIGHT
float CalculateAttenuation(PointLight light, vec3 WorldPos)
{
	// physically correct
	float distance = length(light.position - WorldPos);
	return 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
}

// ----------------------------------------------------------------------------
// SHADOWS
float ShadowCascadeCalculation(vec4 FragPosLightSpace, sampler2DArray shadowmap, int shadowMapIndex, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowmap, vec3(projCoords.xy, shadowMapIndex)).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    float bias = max(0.03 * (1.0 - dot(normal, lightDir)), 0.003);
    // check whether current frag pos is in shadow
    // PCF
    float shadow = 0.0;
    vec3 texelSize = 1.0 / textureSize(shadowmap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 uvSample = projCoords.xy + vec2(x, y) * texelSize.xy;
            float pcfDepth = texture(shadowmap, vec3(uvSample, shadowMapIndex)).r; 
            //shadow += currentDepth - 0.00001 > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return 0.0;
}

// ----------------------------------------------------------------------------
// DIRECTIONAL LIGHT
vec4 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir, vec3 albedo, vec4 FragPosLightSpace, sampler2DArray shadowmap)
{
	// light direction
	vec3 L = normalize(-light.direction);
	// calculate radiance 
	vec3 radiance = vec3(max(dot(L, normal), 0.0)) * light.diffuse * vec3(light.intensity);
	// shadows
	//float shadow = ShadowCascadeCalculation(FragPosLightSpace, shadowmap, 0, normal, L);
    float shadow = 0.0;
	// return lighting
	vec3 lighting = vec3(1.0 - shadow) * radiance * albedo;
    return vec4(lighting, 1.0);
}
// ----------------------------------------------------------------------------
// POINT LIGHT
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo)
{
	// light direction
	vec3 L = normalize(light.position - FragPos);
	// calculate attenuation
	float attenuation = CalculateAttenuation(light, FragPos);
    // calculate radiance 
	vec3 radiance = light.diffuse * light.intensity * attenuation * max(dot(L, normal), 0.0);
	// shadows
	// NEEDS IMPLEMENTATION
    vec3 lighting = radiance * albedo;
    return vec4(lighting, 1.0);
}
// ----------------------------------------------------------------------------
// SPOT LIGHT
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo)
{
	// NEEDS IMPLEMENTATION
	return vec4(0.0);
}
// ----------------------------------------------------------------------------  
