#version 330 core
out vec4 FragColor;

// inputs
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//material
struct Material{
	float ambientBrightness;
	sampler2D diffuseMap;
	sampler2D specularMap;
	vec3 specular;
	float specularIntensity;
	float roughness;
};

// light
struct Light{
	int type;
	vec3 position;
	vec3 direction;
	float intensity;
	vec3 diffuse;
	// spotlight
	float spotSize;
	float penumbraSize;

	// falloff 
	float constant;
	float linear;
	float quadratic;

};

// uniforms
uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
	
	vec3 normal = normalize(Normal);
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		light.quadratic * (distance * distance)); 
	vec3 diffuseMap = vec3(texture(material.diffuseMap, TexCoords));
	vec3 lightDir = normalize(light.position - FragPos);
	vec3 lit;

	// lit component 
	// adding to lighting depending on light type
	if (light.type == 0 ) // point light ** check lighting/Light.h for reference
	{
		lit = diffuseMap * light.diffuse * light.intensity * dot(normal, lightDir) ;
	}
	else if(light.type == 1 ) // direction light
	{
		lightDir = normalize(-light.direction);
		lit = diffuseMap * light.diffuse * light.intensity * dot(normal, lightDir);
		attenuation = 1.0;
	}
	else if(light.type == 2 ) // spot light
	{
		float theta = dot(normalize(-light.direction), lightDir);
		float intensity = 1.0 - clamp(smoothstep(light.spotSize, light.penumbraSize, theta), 0.0, 1.0);
		lit = diffuseMap * light.diffuse * light.intensity * dot(normal, lightDir) * intensity;

	}

	// specular component
	vec3 specMap = vec3(texture(material.specularMap, TexCoords));
	// we reflect the light direction over the normal and measure the dot product between the reflected angle and our eye
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectedLight = normalize(reflect(-lightDir, normal));
	vec3 specularLit = lit * pow(max(dot(viewDir, reflectedLight), 0.0), material.roughness); 
	vec3 specular = specularLit * material.specularIntensity * specMap * attenuation;

	// ambient component
	vec3 ambient = light.diffuse * material.ambientBrightness;

	// out
	FragColor = vec4((max(lit, ambient) * attenuation + specular ), 1.0);
}