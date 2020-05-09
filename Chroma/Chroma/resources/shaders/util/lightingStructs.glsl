// DIRECTIONAL
struct DirectionLight 
{
	vec3 diffuse;
	vec3 direction;
	float intensity;
};

// POINTS
struct PointLight
{
	vec3 diffuse;
	vec3 position;
	float intensity;
	// attenuation
	float constant;
	float linear;
	float quadratic;
	float radius;
};

// SPOTLIGHTS
struct SpotLight
{
	vec3 diffuse;
	vec3 direction;
	float intensity;
	vec3 position;
	// attenuation
	float constant;
	float linear;
	float quadratic;
	// spotlight
	float spotSize;
	float penumbraSize;
};
