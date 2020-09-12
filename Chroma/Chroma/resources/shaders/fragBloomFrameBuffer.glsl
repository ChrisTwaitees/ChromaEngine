#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D u_Background;
uniform sampler2D u_BloomTx;

uniform sampler2D u_DirectLightingShadows;
uniform sampler2D u_IndirectLighting;
uniform sampler2D u_SSR;
uniform sampler2D u_VXGI;
uniform sampler2D u_VXReflectance;
uniform sampler2D u_SSAO;
uniform sampler2D u_MetRoughAO;

// RenderSettings
uniform bool u_Bloom;
uniform float u_BloomAmount;
uniform float u_IndirectContribution;
uniform float u_SSRContribution;
uniform float u_VXIrradiance_Contribution;
uniform float u_VXRradiance_Contribution;
uniform float u_Exposure;
uniform float u_Gamma;

void main()
{
    vec3 MetRoughAO = texture(u_MetRoughAO, TexCoords).rgb;

    // BG
	vec3 backgroundColor = texture(u_Background, TexCoords).rgb;

    // Bloom
    vec3 bloomColor = vec3(0.0f);
    if (u_Bloom)
        bloomColor = texture(u_BloomTx, TexCoords).rgb;

    // Direct Lighting
    vec4 directLighting = texture(u_DirectLightingShadows, TexCoords);

    // Indirect Lighting
    vec3 indirectLighting = texture(u_IndirectLighting, TexCoords).rgb;

    // SSAO
    float SSAO = texture(u_SSAO, TexCoords).r;

	// SSR
	vec4 SSR = texture(u_SSR, TexCoords);
    vec4 SSRColor = vec4(texture(u_DirectLightingShadows, SSR.rg).rgb + texture(u_IndirectLighting, SSR.rg).rgb * vec3(SSAO), SSR.a * (1.0 - MetRoughAO.g));
    if (SSRColor.rgb == vec3(1.0))
        SSRColor.a = 0.0;
    // VXGI
    vec4 VXGI = texture(u_VXGI, TexCoords);


    // Indirect Lighting SSR fallsback to VXGI falls Back to Baked Indirect Lighting
    vec3 AccumulatedIrradiance = mix(mix(indirectLighting.rgb, SSRColor.rgb, SSRColor.a), mix(VXGI.rgb, SSRColor.rgb, SSRColor.a), VXGI.a) * vec3(MetRoughAO.b) ;

    // Final Color 
    vec3 FinalColor = backgroundColor.rgb;
    
    // TONE MAPPING
    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-FinalColor * u_Exposure);
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / u_Gamma));
    //FragColor = vec4(mapped, 1.0);
    FragColor = vec4((directLighting.rgb + AccumulatedIrradiance) * vec3(SSAO), 1.0);
    //FragColor = vec4(vec3(min(SSAO, MetRoughAO.b)), 1.0);

}