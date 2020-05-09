layout (std140) uniform CameraUBO
{
	mat4 projection;
	mat4 view;
};

layout (std140) uniform LightingUBO
{
	// lights
	int numPointLights;
	int numDirectionalLights;
	int numSpotLights;

	// shadows
	int numCascades;
	float cascadeDistances[3];
	mat4 cascadeLightMatrices[3];
};



//layout (std140) uniform ExampleBlock
//{
//    float value;
//    vec3  vector;
//    mat4  matrix;
//    float values[3];
//    bool  boolean;
//    int   integer;
//};  