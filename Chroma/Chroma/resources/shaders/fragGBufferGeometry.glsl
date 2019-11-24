#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gViewPosition;
layout (location = 2) out vec4 gFragPosLightSpace;
layout (location = 3) out vec3 gAlbedo;
layout (location = 4) out vec3 gNormal;
layout (location = 5) out vec3 gViewNormal;
layout (location = 6) out vec3 gMetRoughAO;


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

#include "util/materialStruct.glsl"
uniform Material material;
uniform bool UseNormalMap;

void main()
{             
    // position
    gPosition = fs_in.FragWorldPos;
	gViewPosition = fs_in.FragViewPos;
    // albedo
    gAlbedo = texture(material.texture_albedo1, fs_in.TexCoords).rgb;
	// normals
	if (UseNormalMap  && length(fs_in.ViewTBN[1]) >= 0.5)
	{
		vec3 normalMap = vec3(texture(material.texture_normal1, fs_in.TexCoords));
		gNormal = normalize(fs_in.WorldTBN * normalMap);
		gViewNormal = normalize(fs_in.ViewTBN * normalMap);
	}
	else
	{
		gNormal = fs_in.WorldNormal;
		gViewNormal = fs_in.ViewNormal;
	}
	// metalness roughness ao
    gMetRoughAO = texture(material.texture_MetRoughAO1, fs_in.TexCoords).rgb;
	// shadowmap
	gFragPosLightSpace = fs_in.FragPosLightSpace;
}