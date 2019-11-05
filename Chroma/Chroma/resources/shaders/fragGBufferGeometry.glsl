#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoRoughness;
layout (location = 3) out vec4 gMetalnessSpecular;
layout (location = 4) out vec4 gFragPosLightSpace;
layout (location = 5) out vec3 gViewPosition;
layout (location = 6) out vec3 gViewNormal;


in VS_OUT {
	vec3 FragWorldPos;
	vec3 FragViewPos;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
	vec3 WorldNormal;
	vec3 ViewNormal;
	mat3 WorldTBN;
	mat3 ViewTBN;
} fs_in;

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_roughness1;
	sampler2D texture_metalness1;
	sampler2D texture_normal1;
	sampler2D texture_specular1;
};

uniform Material material;

void main()
{             
    // store the fragment position vector in the first gbuffer texture
    gPosition = fs_in.FragWorldPos;
	gViewPosition = fs_in.FragViewPos;

    // also store the per-fragment normals
	vec3 rawNormalMap = vec3(texture(material.texture_normal1, fs_in.TexCoords));
	vec3 normalMap = normalize(rawNormalMap * 2.0 - 1.0);
	// check if TBN and NormalMap valid
	if (rawNormalMap.g != 0 && length(fs_in.ViewTBN[1]) >= 0.5)	{
		gNormal = normalize(fs_in.WorldTBN * normalMap);
		gViewNormal = normalize(fs_in.ViewTBN * normalMap);
	}
	else{
		gNormal = fs_in.WorldNormal;
		gViewNormal = fs_in.ViewNormal;
	}

    // and the diffuse per-fragment color
    gAlbedoRoughness.rgb = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;
    // store specular roughness in gAlbedoRoughness's alpha component
    gAlbedoRoughness.a = texture(material.texture_roughness1, fs_in.TexCoords).r;
	// store specular metalness in gMetalness color
	gMetalnessSpecular.rgb = texture(material.texture_metalness1, fs_in.TexCoords).rgb;
	gMetalnessSpecular.a = texture(material.texture_specular1, fs_in.TexCoords).g;
	// Shadowmap
	gFragPosLightSpace = fs_in.FragPosLightSpace;
	
}