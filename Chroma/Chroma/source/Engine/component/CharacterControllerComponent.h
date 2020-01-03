#ifndef _CHROMA_CHARACTERCONTROLLER_COMPONENT_H_
#define _CHROMA_CHARACTERCONTROLLER_COMPONENT_H_

// thirdparty
#include <glm/glm.hpp>

// chroma
#include <common/CoreCommon.h>
#include <component/IComponent.h>


class CharacterControllerComponent : public IComponent
{
	glm::vec3 m_Gravity{ 0.0, -9.81, 0.0 };
	float m_Speed{ 10.0 };
	float m_JumpHeight{ 2.0 };

	void ProcessInput();
public:
	CharacterControllerComponent();
	~CharacterControllerComponent();
};

#endif