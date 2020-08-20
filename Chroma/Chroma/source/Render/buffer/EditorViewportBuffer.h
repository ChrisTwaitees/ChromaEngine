#ifndef CHROMA_EDITORVIEWPORTBUFFER_H
#define CHROMA_EDITORVIEWPORTBUFFER_H

#include <buffer/IFramebuffer.h>

namespace Chroma
{
	class EditorViewportBuffer : public IFramebuffer
	{
	public:
		EditorViewportBuffer();
		~EditorViewportBuffer();

	private:
		virtual void Init() override;
		virtual void ResizeBuffers() override;

	};
}
#endif // CHROMA_EDITORVIEWPORTBUFFER_H