// LIGHTS
struct DirLight 
{
	vec3 direction;
	vec3 diffuse;
	float intensity;
};

struct PointLight
{
	vec3 position;
	vec3 diffuse;
	float intensity;
	// attenuation
	float constant;
	float linear;
	float quadratic;
	float radius;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float intensity;
	// attenuation
	float constant;
	float linear;
	float quadratic;
	// spotlight
	float spotSize;
	float penumbraSize;
	vec3 diffuse;
};


// SHADOWS
struct ShadowMap
{
	sampler2D shadowmap1;
};