// LIGHTS
#define MAX_POINT_LIGHTS 20
#define MAX_DIRECTIONAL_LIGHTS 3
#define MAX_SPOT_LIGHTS 20

layout (std140) uniform LightingUBO
{
	// lights
	int numDirectionalLights;
	int numPointLights;
	int numSpotLights;
	// light structs
	DirectionLight UBO_DirLights[MAX_DIRECTIONAL_LIGHTS];
	PointLight UBO_PointLights[MAX_POINT_LIGHTS];
	SpotLight UBO_SpotLights[MAX_SPOT_LIGHTS];


//	// shadows
//	int numCascades;
//	float cascadeDistances[3];
//	mat4 cascadeLightMatrices[3];
};
