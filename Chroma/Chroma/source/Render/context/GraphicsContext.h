#ifndef CHROMA_GRAPHICSCONTEXT_H
#define CHROMA_GRAPHICSCONTEXT_H

#include <common/PrecompiledHeader.h>

namespace Chroma
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static std::unique_ptr<GraphicsContext> Create(void* windowHandle);

	};
}


#endif //CHROMA_GRAPHICSCONTEXT_H