#ifndef _CHROMA_ICON_H
#define _CHROMA_ICON_H

#include <math/Math.h>
#include <component/UIComponent.h>
#include <shader/Shader.h>
#include <texture/Texture.h>


class Icon : public UIComponent
{
public:
	inline float& GetScale() { return m_Scale; }
	inline void SetScale(float const& newScale) {m_Scale = newScale; }

	inline glm::vec3& GetPosition() { return m_Position; }
	inline void SetPosition(glm::vec3 const& newPosition) { 
		m_Position = newPosition; 
		UpdateTransform();
	}

	void SetIconTexture(std::string const& iconPath);
	void SetIconTexture(Texture& newTexture);
	void DrawWithIconTexture(Texture& newTexture);

	void Draw() override;

	Icon(std::string const& iconPath);
	Icon();
	~Icon();

	void BindDrawVAO();
private :
	void Initialize();
	void UpdateTransform();

	void SetupQuad();
	// vertex array / vertex buffer
	unsigned int VAO, VBO;
	glm::mat4 m_ModelMatrix;
	std::string m_IconSourcePath;
	float m_Scale{ 1.0f };
	glm::vec3 m_Position{1.0f};
	Shader m_IconShader{ "resources/shaders/ui/icon/frgIcon.glsl", "resources/shaders/ui/icon/vtxIcon.glsl" };
	Texture m_IconTexture{ "resources/icons/light_icon.png" };
	bool m_IgnoreDepth{ false };
};

#endif