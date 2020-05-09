layout (std140) uniform LightingUBO
{
	// lights
	int numDirectionalLights;
	int numPointLights;
	int numSpotLights;
	// light structs
	//DirLight dirLights[1];


//	// shadows
//	int numCascades;
//	float cascadeDistances[3];
//	mat4 cascadeLightMatrices[3];
};
