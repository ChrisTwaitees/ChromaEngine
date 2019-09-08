#version 330 core
out vec4 FragColor;

// surface info
in vec3 Normal;
in vec3 FragPos;

// light info
uniform vec3 viewPos;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity = 1.0;

// specular
uniform float specularIntensity = 0.5;

// ambient
uniform float ambientIntensity = 0.3f;
uniform vec3 objectColor = vec3(0.5, 0.5, 0.5);
void main()
{
	// light intensity based on facing normal
	vec3 lightDir  = normalize(lightPosition-FragPos);
	vec3 normal = normalize(Normal);
	float facingAmount = lightIntensity * max(dot(normal, lightDir), 0.0);

	// diffuse constribution
	vec3 diffuse = lightColor * facingAmount;

	// specular 
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflectedLightDir = normalize(reflect(-lightDir, normal));
	float specularAmount = pow(max(dot(viewDir, reflectedLightDir), 0.0), 32);
	vec3 specular = lightColor * specularAmount * specularIntensity ;


	// ambient contribution
	vec3 ambient = ambientIntensity * objectColor ;

	// out
	vec3 outColor = (diffuse + specular + ambient) * objectColor;


	FragColor = vec4(outColor, 1.0);
}