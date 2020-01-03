#ifndef _CHROMA_THIRDPERSON_CHARACTERCONTOLLER_H_
#define _CHROMA_THIRDPERSON_CHARACTERCONTOLLER_H_


// chroma
#include <component/CharacterControllerComponent.h>


class ThirdPersonCharacterController : public CharacterControllerComponent
{
	float  m_JumpStrength = 2.0f;
	void ProcessInput() override;
public:

	void Update() override;

	ThirdPersonCharacterController() {};
	~ThirdPersonCharacterController() {};

};


#endif