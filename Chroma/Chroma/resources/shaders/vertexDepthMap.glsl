#version 330 core

layout (location = 0 ) in vec3 aPos;
layout (location = 5) in int[#MAX_VERT_INFLUENCES] aJointIDs;
layout (location = 6) in float[#MAX_VERT_INFLUENCES] aJointWeights;


uniform mat4 lightSpaceMatrix;
uniform mat4 model;

// CONSTS
const int MAX_JOINTS = #MAX_JOINTS;

// skinning
uniform bool isSkinned;
uniform mat4 aJoints[MAX_JOINTS];

void main()
{
	// Skinning
	vec4 LocalPosition = vec4(0.0);
	if (isSkinned)
	{
		for(int i = 0 ; i < #MAX_VERT_INFLUENCES ; i++)
		{
			mat4 jointTransform = aJoints[aJointIDs[i]];
			vec4 posePosition = jointTransform * vec4(aPos, 1.0);
			LocalPosition += posePosition * aJointWeights[i];			
		}
	}
	else
		LocalPosition =  vec4(aPos , 1.0);

	gl_Position = lightSpaceMatrix * model * LocalPosition;
}