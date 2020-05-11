#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
  
in vec2 TexCoords;

// BUFFERS
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMetRoughAO;
uniform sampler2D gFragPosLightSpace;
uniform sampler2DArray gShadowmap;
uniform sampler2D SSAO;

// UNIFORMS
//IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;

// LIGHTING
#include "util/lightingStructs.glsl"
// ubos
#include "util/uniformBufferLighting.glsl"
#include "util/uniformBufferCamera.glsl"
// Lighting Functions
#include "util/PBRLightingFuncsDeclaration.glsl"

void main()
{         
    // retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
	vec3 MetRoughAO = texture(gMetRoughAO, TexCoords).rgb;
	float Metalness = MetRoughAO.r;
    float Roughness = MetRoughAO.g;
	float AO = MetRoughAO.b;
	vec4 FragPosLightSpace = texture(gFragPosLightSpace, TexCoords).rgba;
	float SSAO = texture(SSAO, TexCoords).r;

	// Attrs
	vec3 viewDir = normalize(cameraPosition - FragPos);

	// LIGHTING
	//------------------------------------------------------------------------
	// PBR calculates irradiance, denoted by Lo
	vec4 Lo;
	// Directional Lights
	for(int i = 0; i < numDirectionalLights ; i++)
		Lo += CalcDirLight(UBO_DirLights[i], Normal, viewDir, Albedo, Roughness, Metalness, FragPosLightSpace, gShadowmap);
	// Point Lights
	for(int i = 0; i < numPointLights ; i++)
		Lo += CalcPointLight(UBO_PointLights[i], Normal, viewDir, FragPos, Albedo, Roughness, Metalness, FragPosLightSpace, gShadowmap);

	// AMBIENT
	//------------------------------------------------------------------------
	vec3 Ambient = CalcAmbientLight(irradianceMap, prefilterMap, brdfLUT, Normal, viewDir, Albedo, Roughness, Metalness, AO, Lo.a);

	// COMBINE
	//------------------------------------------------------------------------
	vec3 color = (Ambient + Lo.rgb) * SSAO;

	// OUT
	//------------------------------------------------------------------------
	FragColor = vec4(vec3(color), 1.0);

	// POST FX
	//------------------------------------------------------------------------
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}  

#include "util/PBRLightingFuncsImplementation.glsl"