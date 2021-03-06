// UTILS
const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;
// ----------------------------------------------------------------------------
// POINT LIGHT
float CalculateAttenuation(PointLight light, vec3 WorldPos)
{
	// allows for more control not physically correct
	// physically correct : ;
	float distance = length(light.position - WorldPos);
	return 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//return  1.0 / (distance * distance);
}
// ----------------------------------------------------------------------------
// SHADOWS

// PCF Samples
vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);  


float ShadowCascadeCalculation(vec4 FragPosLightSpace, sampler2DArray shadowmap, int shadowMapIndex, vec3 normal, vec3 lightDir)
{

    // perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        return 0.0;
    // bias
    float shadowBias = 0.00001;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // sample the shadowmap
    vec3 texelSize = 1.0 / textureSize(shadowmap, 0);
    float u;
    float v;
    float shadow = 0;
    for (v = -1.5; v <= 1.5; v += 1.0)
    {
        for (u = -1.5; u <= 1.5; u += 1.0)
        {
            vec2 uvSample = projCoords.xy + vec2(u, v) * texelSize.xy;
            float pcfDepth = texture(shadowmap, vec3(uvSample,shadowMapIndex)).r; 
            shadow += currentDepth - shadowBias > pcfDepth  ? 1.0 : 0.0;  
        }
    }
    shadow /=16.0;
    return shadow;  

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
    //float bias = max(0.03 * (1.0 - dot(normal, lightDir)), 0.003);
    // check whether current frag pos is in shadow
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
			vec2 uvSample = projCoords.xy + vec2(x, y) * texelSize.xy;
            float pcfDepth = texture(shadowmap, uvSample).r; 
            shadow += currentDepth - 0.00001 > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

// ----------------------------------------------------------------------------
// PBR
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}
// ----------------------------------------------------------------------------
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec4 PBRLighting(vec3 lightRadiance, vec3 Normal, vec3 H, vec3 ViewDir, vec3 LightDir, vec3 albedo, float metalness, float roughness)
{
	// Normal Distribution (D)
	float NDF = DistributionGGX(Normal, H, roughness); 
	// Geometry Distribution (G)
	float G   = GeometrySmith(Normal, ViewDir, LightDir, roughness);
	// Fresnel (F)
	vec3 F0   = vec3(0.04); 
	F0        = mix(F0, albedo, metalness);
	vec3 F    = FresnelSchlick(max(dot(H, ViewDir), 0.0), F0);

	// DFG
	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(Normal, ViewDir), 0.0) * max(dot(Normal, LightDir), 0.0);
	vec3 specular     = numerator / max(denominator, 0.001); 

	// Calculate refraction(diffuse contribution) and specular
	vec3 kS = F; // specular
	vec3 kD = vec3(1.0) - kS; // diffuse
  
	kD *= 1.0 - metalness;

	// final light calc
	float NdotL = max(dot(Normal, LightDir), 0.0);        
    return vec4((kD * albedo / PI + specular) * lightRadiance * NdotL, NdotL * lightRadiance.r);
}
// ----------------------------------------------------------------------------
// DIRECTIONAL LIGHT
vec4 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metalness, vec4 FragPosLightSpace, sampler2DArray shadowmap)
{
	// light direction
	vec3 L = normalize(-light.direction);
	// normalize viewDir and light direction
	vec3 H = normalize(viewDir + L);
	// calculate radiance 
	vec3 radiance = light.diffuse * light.intensity;
	// calculate PBR diffuse and specular components
	vec4 lighting = PBRLighting(radiance, normal, H, viewDir, L, albedo, metalness, roughness);
	// shadows
	float shadow = ShadowCascadeCalculation(FragPosLightSpace, shadowmap, 0,  normal, L);
	// return 
	return vec4((1.0 - shadow) * lighting);
}
// ----------------------------------------------------------------------------
// POINT LIGHT
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo, float roughness, float metalness, vec4 FragPosLightSpace, sampler2DArray shadowmap)
{
	// light direction
	vec3 L = normalize(light.position - FragPos);
	// normalize viewDir and light direction
	vec3 H = normalize(viewDir + L);
	// calculate attenuation
	float attenuation = CalculateAttenuation(light, FragPos);
	// calculate radiance 
	vec3 radiance = light.diffuse * light.intensity * attenuation;
	// calculate PBR diffuse and specular components
	vec4 lighting = PBRLighting(radiance, normal, H, viewDir, L, albedo, metalness, roughness);
	// shadows
//	float shadow = ShadowCascadeCalculation(FragPosLightSpace, shadowmap, 0, normal, L);
	// return 
	return vec4(lighting);
}
// ----------------------------------------------------------------------------
// SPOT LIGHT
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos, vec3 albedo, float roughness, float metalness, vec4 FragPosLightSpace, sampler2DArray shadowmap)
{
//	// light direction
//	vec3 L = normalize(light.position - FragPos);
//	// normalize viewDir and light direction
//	vec3 H = normalize(viewDir + L);
//	// calculate attenuation
//	float attenuation = CalculateAttenuation(light, FragPos);
//	// calculate radiance 
//	vec3 radiance = light.diffuse * light.intensity * attenuation;
//	// calculate PBR diffuse and specular components
//	vec4 lighting = PBRLighting(radiance, normal, H, viewDir, L, albedo, metalness, roughness);
//	// shadows
//	float shadow = ShadowCascadeCalculation(FragPosLightSpace, shadowmap, 0, normal, L);
	// return 
	return vec4(0.0);
}
// ----------------------------------------------------------------------------
// FresnelSchlickRougness
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   
// ----------------------------------------------------------------------------
// AMBIENT LIGHT
vec3  CalcAmbientLight(samplerCube irradianceMap, samplerCube prefilterMap, sampler2D brdfLUT, vec3 normal, vec3 viewDir, vec3 albedo, float roughness, float metalness, float ao, float shadows)
{
    // ambient lighting (we now use IBL as the ambient term)
	// DIFFUSE
	vec3 F0   = vec3(0.04); 
	F0        = mix(F0, albedo, metalness);
	vec3 F = fresnelSchlickRoughness(max(dot(normal, viewDir), 0.0), F0, roughness);
    vec3 kD   = 1.0 - F;
    kD       *= 1.0 - metalness;	  
    vec3 irradiance  = texture(irradianceMap, normal).rgb;
    vec3 diffuse     = irradiance * albedo;
	// SPECULAR
    vec3 R                = reflect(-viewDir, normal); 
	vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf             = texture(brdfLUT, vec2(max(dot(normal, viewDir), 0.0), roughness)).rg;
    vec3 specular         = prefilteredColor * (F * brdf.x + brdf.y);
	specular *= max(shadows , 0.4);
	// AMBIENT
	vec3 ambient = (kD * diffuse + specular) * ao;
	// return 
	return  ambient;
}