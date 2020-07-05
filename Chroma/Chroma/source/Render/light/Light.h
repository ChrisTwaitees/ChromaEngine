#ifndef _LIGHT_H_
#define _LIGHT_H_

//common
#include <common/PrecompiledHeader.h>
#include <component/IComponent.h>
#include <texture/Texture.h>


namespace Chroma
{
	class Light : public IComponent
	{

	public:
		// icomponent interface
		virtual void Init();
		virtual void OnUpdate();
		virtual void Destroy();
		virtual void Serialize(ISerializer*& serializer) override;

		// editor functions
	#ifdef EDITOR
		void DrawIcon(Texture& iconTexture);
		UID m_IconUID;
	#endif

		// methods
		// setters
		void SetLightType(Chroma::Type::Light light_type) { m_LightType = light_type; UpdatePointRadius();};
		void SetDiffuse(glm::vec3 color) { m_Diffuse = color; UpdatePointRadius();};
		void SetIntensity(float intensity_val) { m_Intensity = intensity_val; UpdatePointRadius();};
		void SetTranslation(glm::vec3 newPosition) { m_Position = newPosition; };
		void SetDirection(glm::vec3 direction_val) { m_Direction = glm::normalize(direction_val); };
		// setters- attenuation
		void setConstant(float constant_val) { m_Constant = constant_val; UpdatePointRadius();};
		void setLinear(float linear_val) { m_Linear = linear_val; UpdatePointRadius();};
		void setQuadratic(float quadratic_val) { m_Quadratic = quadratic_val; UpdatePointRadius();};
		// setters - spotlight
		void setPenumbraSize(float penumbra_val) { m_PenumbraSize = penumbra_val; };
		void setSpotSize(float spotSize_val) {m_SpotSize = spotSize_val;};

		// getters
		Chroma::Type::Light GetLightType() const { return m_LightType; };
		// getters - directional
		glm::vec3 GetDirection() { return m_Direction; };
		// getters - pointlights
		glm::vec3 GetPosition() { return m_Position; };
		float GetIntensity() { return m_Intensity; };
		glm::vec3 GetDiffuse() { return m_Diffuse; };
		float GetRadius() { return m_Radius; };
		inline float GetConstant() const { return m_Constant; }
		inline float GetLinear() const { return m_Linear; }
		inline float GetQuadratic() const { return m_Quadratic; }
		// getters - spotlights
		float GetSpotSize() { return m_SpotSize; };
		float GetPenumbraSize() { return m_PenumbraSize; };

		// constructors
		Light();
		Light(Chroma::Type::Light type_val) : m_LightType{ type_val } { Init(); };
		Light(Chroma::Type::Light type_val, float intensity_val) : m_LightType{ type_val }, m_Intensity{ intensity_val }{ Init(); };
		Light(glm::vec3 position_val, Chroma::Type::Light type_val) : m_LightType{ type_val }, m_Position{ position_val } { Init(); };
		Light(Chroma::Type::Light type_val, glm::vec3 direction_val) : m_LightType{ type_val }, m_Direction{ direction_val } {Init(); };
		Light(Chroma::Type::Light type_val, glm::vec3 position_val, glm::vec3 direction_val, float intensity_val) : m_Position{ position_val }, m_Direction{ direction_val }, m_Intensity{ intensity_val }, m_LightType{type_val} {Init(); };
		Light(Chroma::Type::Light type_val, glm::vec3 direction_val, float intensity_val) :  m_Direction{ direction_val }, m_Intensity{ intensity_val }, m_LightType{ type_val } { Init(); };
		~Light();

	protected:
		// type
		Chroma::Type::Light m_LightType{ Chroma::Type::Light::kPointLight};

		// attenuation
		float m_Constant{ 1.0f };
		float m_Linear{ 1.0f };
		float m_Quadratic{ 1.0f };
		float m_LightMax = {1.0f};
		float m_Radius = { 2.5f };
		void UpdatePointRadius();

		float m_Intensity{ 1.0f };
		// transforms
		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Direction{ 0.0f, -1.0f, 0.0f };
		glm::vec3 m_Diffuse{ 1.0f, 1.0f, 1.0f };

		// spotlight
		float m_SpotSize{ glm::cos(glm::radians(8.5f)) };
		float m_PenumbraSize{ glm::cos(glm::radians(17.5f)) };
	};
}
#endif