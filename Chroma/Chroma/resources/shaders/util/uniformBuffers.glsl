layout (std140) uniform CameraMatrices
{
	mat4 projection;
	mat4 view;
};

//layout (std140) uniform Lights
//{
//	int numPointLights;
//	int numDirectionalLights;
//	int numSpotLights;
//};
//
//layout (std140) uniform Shadows
//{
//	int numCascades;
//	float cascadeDistances[3];
//	mat4 cascadeLightMatrices[3];
//};

//layout (std140) uniform ExampleBlock
//{
//    float value;
//    vec3  vector;
//    mat4  matrix;
//    float values[3];
//    bool  boolean;
//    int   integer;
//};  