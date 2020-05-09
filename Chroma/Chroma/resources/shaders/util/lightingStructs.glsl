// LIGHTS
struct DirLight 
{
	float intensity;
	vec3 diffuse;
	vec3 direction;
};

struct PointLight
{
	float intensity;
	vec3 diffuse;
	vec3 position;
	// attenuation
	float constant;
	float linear;
	float quadratic;
	float radius;
};

struct SpotLight
{
	float intensity;
	vec3 diffuse;
	vec3 direction;
	vec3 position;
	// attenuation
	float constant;
	float linear;
	float quadratic;
	// spotlight
	float spotSize;
	float penumbraSize;
};