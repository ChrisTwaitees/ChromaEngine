#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoRoughness;
layout (location = 3) out vec4 gMetalnessSpecular;
layout (location = 4) out vec4 gFragPosLightSpace;


in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
	mat3 TBN;
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
    gPosition = vec4(fs_in.FragPos, 1.0);
    // also store the per-fragment normals into the gbuffer
	vec3 normalMap = vec3(texture(material.texture_normal1, fs_in.TexCoords));
	vec3 norm = normalize(normalMap * 2.0 - 1.0);
	norm = normalize(fs_in.TBN * norm);
	gNormal = normalMap.g == 0.0 ?  vec4(normalize(fs_in.Normal), 1.0) : vec4(normalize(norm), 1.0);

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