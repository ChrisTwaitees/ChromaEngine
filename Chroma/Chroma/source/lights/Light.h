#ifndef _LIGHT_H_
#define _LIGHT_H_
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../shaders/Shader.h"

class Light
{
public:
	//members
	enum TYPE { POINT = 0, DIRECTIONAL = 1, SPOT= 2 };
	TYPE type{ TYPE::POINT };
	float intensity{ 1.0f };
	// transforms
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 direction{ 0.0f, -1.0f, 0.0f };
	glm::vec3 diffuse{ 1.0f, 1.0f, 1.0f };
	// attenuation
	float constant{ 1.0f };
	float linear{ 0.09f };
	float quadratic{ 0.032f };
	// spotlight
	float spotSize{ glm::cos(glm::radians(8.5f)) };
	float penumbraSize{ glm::cos(glm::radians(17.5f)) };
	// methods
	// setters
	void setType(Light::TYPE light_type) { type = light_type; };
	void setDiffuse(glm::vec3 color) { diffuse = color; };
	void setIntensity(float intensity_val) { intensity = intensity_val; };
	void setDirection(glm::vec3 direction_val) { direction = glm::normalize(direction_val); };
	// setters- attenuation
	void setConstant(float constant_val) { constant = constant_val; };
	void setLinear(float linear_val) { linear = linear_val; };
	void setQuadratic(float quadratic_val) { quadratic = quadratic_val; };
	// setters - spotlight
	void setPenumbraSize(float penumbra_val) { penumbraSize = penumbra_val; };
	void setSpotSize(float spotSize_val) {spotSize = spotSize_val;};

	// getters
	TYPE getType() const { return type; };

	// uniform updates
	void updateUniforms(Shader& shader);

	// constructors
	Light();
	Light(TYPE type_val) : type{ type_val } {};
	Light(TYPE type_val, float intensity_val) : type{ type_val }, intensity{ intensity_val }{ };
	Light(glm::vec3 position_val, TYPE type_val) : type{ type_val }, position{ position_val } {};
	Light(TYPE type_val, glm::vec3 direction_val) : type{ type_val }, direction{ direction_val } { };
	Light(TYPE type_val, glm::vec3 position_val, glm::vec3 direction_val, float intensity_val) : position{ position_val }, direction{ direction_val }, intensity{ intensity_val }, type{type_val} {};
	Light(TYPE type_val, glm::vec3 direction_val, float intensity_val) :  direction{ direction_val }, intensity{ intensity_val }, type{ type_val } { };
	~Light();
};
#endif