#ifndef _CHROMA_ICON_H
#define _CHROMA_ICON_H

#include <model/NDCPlanePrimitive.h>
#include <math/Math.h>
#include <component/UIComponent.h>

class Icon : public NDCPlanePrimitive, public UIComponent
{
public:
	inline float& GetScale() { return m_Scale; }
	inline void SetScale(float const& newScale) {
		m_Scale = newScale; UpdateTransform();
	}

	inline glm::vec3& GetPosition() { return m_Position; }
	inline void SetPosition(glm::vec3 const& newPosition) { 
		m_Position = newPosition; 
		UpdateTransform();
	}

	void SetIconTexture(std::string const& iconPath);
	void SetIconTexture(Texture& newTexture);

	void Draw() override;

	Icon(std::string const& iconPath);
	Icon();
	~Icon();

private :
	void Initialize();
	void UpdateTransform();

	glm::mat4 m_ModelMatrix;
	std::string m_IconDirectory;
	float m_Scale{ 1.0f };
	glm::vec3 m_Position{0.0f};
	Shader m_IconShader{ "resources/shaders/ui/icon/frgIcon.glsl", "resources/shaders/ui/icon/vtxIcon.glsl" };
	Texture m_IconTexture{ "resources/icons/light_icon.png" };
	bool m_IgnoreDepth{ false };
};

#endif