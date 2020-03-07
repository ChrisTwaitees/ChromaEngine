#pragma once

#include <UI/core/UI.h>
#include <uid/UID.h>

namespace Chroma
{
	class AnimationEditorUI : public UI
	{
	public:
		static void Init();
		static void Draw();

		static void Open();
		static void Close();

	private:
		static bool m_IsOpen;

	};
}



