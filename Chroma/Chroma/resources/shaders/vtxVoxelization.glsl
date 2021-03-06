#version 450 core

layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 aNormal;
layout (location = 2 ) in vec2 aTexCoords;
layout (location = 5) in int[#MAX_VERT_INFLUENCES] aJointIDs;
layout (location = 6) in float[#MAX_VERT_INFLUENCES] aJointWeights;

out VS_OUT{
	out vec3 worldPositionGeom;
	out vec3 normalGeom;
	out vec2 texCoordsGeom;
	out vec4 posLightSpaceGeom;
} vs_out;

// CONSTS
const int MAX_JOINTS = #MAX_JOINTS;

// UNIFORMS
#include "util/uniformBufferCamera.glsl"
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
// skinning
uniform bool isSkinned;
uniform mat4 aJoints[MAX_JOINTS];


void main()
{   
	vec4 LocalPosition = vec4(aPos, 1.0);

	// Skinning
	if (isSkinned)
	{
		mat4 BoneTransform = mat4(1.0);
		for(int i = 0 ; i < #MAX_VERT_INFLUENCES ; i++)
		{
			BoneTransform += aJoints[aJointIDs[i]] * aJointWeights[i];
		}
		vs_out.worldPositionGeom = vec3(model * BoneTransform * LocalPosition);
		vs_out.normalGeom = normalize(mat3(transpose(inverse(model * BoneTransform))) * aNormal);

	}
	else
	{
		vs_out.worldPositionGeom = vec3(model * LocalPosition);
		vs_out.normalGeom = normalize(mat3(transpose(inverse(model))) * aNormal);
	}

	vs_out.texCoordsGeom = aTexCoords;
	vs_out.posLightSpaceGeom = lightSpaceMatrix * vec4(vs_out.worldPositionGeom,1.0);
    gl_Position = projection * view * model * LocalPosition;
}