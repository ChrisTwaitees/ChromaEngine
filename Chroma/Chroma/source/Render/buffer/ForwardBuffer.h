#ifndef _CHROMA_FORWARD_BUFFER_
#define _CHROMA_FORWARD_BUFFER_
#include <buffer/IFramebuffer.h>
#include <scene/Scene.h>

#include <ibl/IBL.h>

namespace Chroma
{
	class ForwardBuffer : public IFramebuffer
	{
		// scene
		IFramebuffer* m_PostFXBuffer{ nullptr };

		// functions
		void Initialize() override;

		void RenderForwardComponents();
		void RenderTransparency();
		void DrawQuad();

	public:
		void Draw() override;
		void ResizeBuffers() override;

		ForwardBuffer(IFramebuffer* const& postFXBuffer);
		~ForwardBuffer();
	};
}

#endif