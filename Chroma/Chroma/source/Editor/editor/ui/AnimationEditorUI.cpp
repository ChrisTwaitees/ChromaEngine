#include "AnimationEditorUI.h"

#include <animation/AnimationStateMachine.h>

namespace Chroma
{
	bool AnimationEditorUI::m_IsOpen;


	void AnimationEditorUI::Init()
	{
		// vars
		m_IsOpen = false;
	}

	void AnimationEditorUI::Draw()
	{
		if (m_IsOpen)
			CHROMA_INFO("Drawing animation menu");
	}
	void AnimationEditorUI::Open()
	{
		m_IsOpen = true;
	}
	void AnimationEditorUI::Close()
	{
		m_IsOpen = false;
	}
}