#ifndef CHROMA_VXGIBUFFER_H
#define CHROMA_VXGIBUFFER_H

#include "buffer/IFramebuffer.h"

namespace Chroma
{
	class VXGIBuffer : public IFramebuffer
	{
	public:
		VXGIBuffer() { Init(); }
		~VXGIBuffer() { delete m_ScreenShader; };

	private:

	};
}


#endif // CHROMA_VXGIBUFFER_H