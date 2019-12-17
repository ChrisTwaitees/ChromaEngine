#version 330 core
layout (location = 0 ) in vec3 aPos;
layout (location = 1 ) in vec3 aNormal;
layout (location = 2 ) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  
layout (location = 5) in int[#MAX_VERT_INFLUENCES] aJointIDs;
layout (location = 6) in float[#MAX_VERT_INFLUENCES] aJointWeights;


out VS_OUT{
	vec3 FragWorldPos;
	vec3 FragViewPos;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
	vec3 WorldNormal;
	vec3 ViewNormal;
	mat3 WorldTBN;
	mat3 ViewTBN;
} vs_out;

// CONSTS
const int MAX_JOINTS = #MAX_JOINTS;

// UNIFORMS
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
// skinning
uniform bool isSkinned;
uniform mat4 aJoints[MAX_JOINTS];


void main()
{    
	// Skinning
	vec4 LocalPosition;
	mat4 BoneTransform =mat4(1.0);
	if (isSkinned)
	{
		for(int i = 0 ; i < #MAX_VERT_INFLUENCES ; i++)
		{
			BoneTransform += aJoints[aJointIDs[i]] * aJointWeights[i];
		}
		LocalPosition =  BoneTransform * vec4( aPos , 1.0);
	}
	else
		LocalPosition =  vec4(aPos , 1.0);

	// world and view, normals and positions
    vs_out.FragWorldPos = vec3(model * LocalPosition) ;
	vs_out.FragViewPos = vec3(view * vec4(vs_out.FragWorldPos, 1.0));
    vs_out.WorldNormal =  transpose(  inverse(mat3(model))) * aNormal;
	vs_out.ViewNormal =  transpose(inverse(mat3( view * model))) * aNormal;

	// uvs and shadowmapping lightspace
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(  vs_out.FragWorldPos , 1.0);
	
	// tbn
	vec3 T = normalize(vec3( model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3( model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3( model * vec4(aNormal,    0.0)));

	// world and view tbn
	vs_out.WorldTBN = mat3(T, B, N);
	vs_out.ViewTBN = mat3(view) * vs_out.WorldTBN;
	
    gl_Position = projection * view * model * LocalPosition;
}