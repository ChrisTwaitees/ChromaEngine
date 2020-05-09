#ifndef _LIGHT_H_
#define _LIGHT_H_

//common
#include <common/PrecompiledHeader.h>
#include <component/IComponent.h>
#include <texture/Texture.h>

class Light : public IComponent
{

public:
	// icomponent interface
	virtual void Init();
	virtual void Update();
	virtual void Destroy();
	virtual void Serialize(ISerializer*& serializer) override;

	// editor functions
#ifdef EDITOR
	void DrawIcon(Texture& iconTexture);
	UID m_IconUID;
#endif

	//members
	enum TYPE { POINT = 0, DIRECTIONAL = 1, SPOT= 2, SUNLIGHT=3 };
	// type
	TYPE type{ TYPE::POINT };

	// attenuation
	float m_Constant{ 1.0f };
	float m_Linear{ 0.07f };
	float m_Quadratic{ 1.8f };
	float m_LightMax = {1.0f};
	float m_Radius = { 2.5f };

	// methods
	// setters
	void SetType(Light::TYPE light_type) { type = light_type; };
	void setDiffuse(glm::vec3 color) { m_Diffuse = color; };
	void setIntensity(float intensity_val) { m_Intensity = intensity_val; };
	void SetTranslation(glm::vec3 newPosition) { m_Position = newPosition; };
	void setDirection(glm::vec3 direction_val) { m_Direction = glm::normalize(direction_val); };
	// setters- attenuation
	void setConstant(float constant_val) { m_Constant = constant_val;  };
	void setLinear(float linear_val) { m_Linear = linear_val; };
	void setQuadratic(float quadratic_val) { m_Quadratic = quadratic_val; };
	// setters - spotlight
	void setPenumbraSize(float penumbra_val) { penumbraSize = penumbra_val; };
	void setSpotSize(float spotSize_val) {spotSize = spotSize_val;};

	// getters
	TYPE getType() const { return type; };
	std::string GetTypeString() const;
	// getters - directional
	glm::vec3 GetDirection() { return m_Direction; };
	// getters - pointlights
	glm::vec3 GetPosition() { return m_Position; };
	float GetIntensity() { return m_Intensity; };
	glm::vec3 GetDiffuse() { return m_Diffuse; };
	float getRadius() { updatePointRadius(); return m_Radius; };
	// getters - spotlights
	float getSpotSize() { return spotSize; };
	float getPenumbraSize() { return penumbraSize; };

	// constructors
	Light();
	Light(TYPE type_val) : type{ type_val } { Init(); };
	Light(TYPE type_val, float intensity_val) : type{ type_val }, m_Intensity{ intensity_val }{ Init(); };
	Light(glm::vec3 position_val, TYPE type_val) : type{ type_val }, m_Position{ position_val } { Init(); };
	Light(TYPE type_val, glm::vec3 direction_val) : type{ type_val }, m_Direction{ direction_val } {Init(); };
	Light(TYPE type_val, glm::vec3 position_val, glm::vec3 direction_val, float intensity_val) : m_Position{ position_val }, m_Direction{ direction_val }, m_Intensity{ intensity_val }, type{type_val} {Init(); };
	Light(TYPE type_val, glm::vec3 direction_val, float intensity_val) :  m_Direction{ direction_val }, m_Intensity{ intensity_val }, type{ type_val } { Init(); };
	~Light();

protected:

	void updatePointRadius();

	float m_Intensity{ 1.0f };
	// transforms
	glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Direction{ 0.0f, -1.0f, 0.0f };
	glm::vec3 m_Diffuse{ 1.0f, 1.0f, 1.0f };

	// spotlight
	float spotSize{ glm::cos(glm::radians(8.5f)) };
	float penumbraSize{ glm::cos(glm::radians(17.5f)) };
};
#endif